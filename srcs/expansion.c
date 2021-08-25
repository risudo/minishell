#include "../includes/parse.h"

static char *ft_getenv(t_envlist *elst, char *search)//あきやまさんのを使う
{
	while (elst)
	{
		if (ft_strncmp(elst->key, search, ft_strlen(search) + 1) == 0)
			return (elst->value);
		elst = elst->next;
	}
	return (NULL);
}

char	*get_expansioned_str(char *str1, char *str2, char *str3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(str1, str2);//*ラッパー使う
	ret = ft_strjoin(tmp, str3);
	xfree(tmp);
	return (ret);
}

// 環境変数を展開してclist->strに置き換える
void	expansion_key(t_cmdlist *clist, t_envlist *envlist, char *p_doller)
{
	char	*key;
	char	*value;
	char	*front_key;
	char	*back_key;
	int		i;

	i = 1;
	front_key = ft_xsubstr(clist->str, 0, p_doller - clist->str);
	while (p_doller[i] != ' ' && p_doller[i] != '$' && p_doller[i] != '\"' && p_doller[i] != '\0')
		i++;
	key = ft_xsubstr(p_doller, 1, i - 1);
	back_key = ft_xsubstr(p_doller + i, 0, ft_strlen(p_doller + i));
	value = ft_getenv(envlist, key);
	xfree(clist->str);
	clist->str = get_expansioned_str(front_key, value, back_key);
	xfree(front_key), xfree(key), xfree(back_key);
}

size_t	ft_strlen_excluded_quot(char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			str++;
			continue ;
		}
		len++;
		str++;
	}
	return (len);
}

char	*get_new_flag(char **quot, char *end_quot)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = ft_xcalloc(ft_strlen(*quot), sizeof(char));
	while ((*quot)[j])
	{
		if (j == end_quot - *quot || j == end_quot - *quot - 1)
		{
			j++;
			continue ;
		}
		ret[i++] = (*quot)[j++];
	}
	xfree(*quot);
	return (ret);
}

// return : the new flag
char	*get_removed_endflag(char **quot, char flag)
{
	char	*end_quot;
	// char	new_flag;

	end_quot = ft_strchr(*quot, flag);
	while (end_quot != NULL)
	{
		*quot = get_new_flag(quot, end_quot);
		end_quot = ft_strchr(*quot, flag);
	}
	return (*quot);
}

//クォーテーションを削除する
//ここの引数はcmdlistじゃないとダメそう
void	clear_quot(t_cmdlist *clist)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	clist->quot = get_removed_endflag(&clist->quot, '1');
	clist->quot = get_removed_endflag(&clist->quot, '2');
	new_str = ft_xcalloc(ft_strlen_excluded_quot(clist->str) + 1, sizeof(char));
	while (clist->str[i])
	{
		if (clist->str[i] == '\'' || clist->str[i] == '\"')
		{
			i++;
			continue ;
		}
		new_str[j++] = clist->str[i++];
	}
	xfree(clist->str);
	clist->str = new_str;
}

	//str[i] == ' '
char	*get_before_space(char *str, int i)
{
	char	*ret;

	ret = ft_substr(str, 0, i);
	return (ret);
}

t_cmdlist	*insert_new_cmdlist(t_cmdlist *clist, int i)
{
	t_cmdlist	*new;
	char		*new_str;
	int			len;

	new = ft_xcalloc(1, sizeof(*new));
	len = 0;
	while (clist->str[i] == ' ')
		i++;
	while (clist->str[i + len] != '\0')
		len++;
	new_str = ft_substr(clist->str, i, len);//*
	new->str = new_str;
	new->prev = clist;
	new->quot = get_quot_flag(new->str);
	// clist->next->prev = new;
	new->next = clist->next;
	clist->next = new;
	return (clist);
}

void	serch_new_space(t_cmdlist *clist)
{
	int		i;
	char	*str;

	i = 0;
	while (clist->str[i])
	{
		if (clist->str[i] == ' ' && clist->quot[i] == '0')
		{
			//clist->strを書き換える
			str = get_before_space(clist->str, i);
			//スペースの後ろを切り取る
			clist = insert_new_cmdlist(clist, i);
			xfree(clist->str);
			clist->str = str;
			xfree(clist->quot);
			clist->quot = get_quot_flag(clist->str);
			i--;
		}
		i++;
	}
}

// cmdlistを回して、環境変数を展開、クォーテーション削除を行う
void	serch_env_cmdlist(t_cmdlist *clist, t_envlist *envlist)
{
	char	*p_doller;

	while (clist)
	{
		p_doller = ft_strchr(clist->str, '$');
		//シングルクォートで囲われていない'$'がなくなるまでループ
		while (p_doller != NULL && clist->quot[p_doller - clist->str] != 'S')
		{
			//変数展開
			expansion_key(clist, envlist, p_doller);
			// quotを更新
			xfree(clist->quot);
			clist->quot = get_quot_flag(clist->str);
			p_doller = ft_strchr(clist->str, '$');
		}
		if (ft_strchr(clist->quot, '1') || ft_strchr(clist->quot, '2'))
			clear_quot(clist);//クォーテーションを消して、quotフラグを更新
		//クォートで囲まれていないスペースがあったらリストを追加していい感じにする
		serch_new_space(clist);
		clist = clist->next;
	}
}

// dataを回してcmdlistとiolistに展開する必要のある環境変数があるか確認する
t_execdata	*expansion(t_execdata *data)
{
	t_execdata	*head;

	head = data;
	while (data)
	{
		serch_env_cmdlist(data->clst, data->elst);
		serch_env_iolist(data->iolst, data->elst);
		data = data->next;
	}
	return (data);
}