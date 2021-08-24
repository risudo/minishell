#include "../includes/parse.h"

char *ft_getenv(t_envlist *elst, char *search)//rakiyamaのを使う
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
	printf("%s\n", clist->str);
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

size_t	get_new_strlen(char *str)
{
	size_t	ret;

	ret = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			str++;
			continue ;
		}
		ret++;
		str++;
	}
	return (ret);
}

char	*get_new_flag(t_cmdlist *cmdlist, char *end_quot)
{
	char	*ret;
	char	*head_quot;
	int		i;
	int		j;

	i = 0;
	j = 0;
	head_quot = cmdlist->quot;
	printf("quot: %s\n", head_quot);
	printf("len: %zu\n", ft_strlen(head_quot));
	ret = ft_xcalloc(ft_strlen(cmdlist->quot), sizeof(char));
	while (cmdlist->quot[j])
	{
		if (j == end_quot - head_quot || j == end_quot - head_quot - 1)
		{
			j++;
			continue ;
		}
		ret[i] = cmdlist->quot[j];
		i++;
		j++;
	}
	xfree(head_quot);
		printf("ret: %s\n", ret);
	return (ret);
}

// 削除後のquotフラグを作成してreturn
char	*get_deleted_endflag(t_cmdlist *clist, char flag)
{
	char	*end_quot;
	// char	new_flag;

	end_quot = ft_strchr(clist->quot, flag);
	while (end_quot != NULL)
	{
		clist->quot = get_new_flag(clist, end_quot);
		end_quot = ft_strchr(clist->quot, flag);
	}
	return (clist->quot);
}


//クォーテーションを削除する
void	clear_quot(t_cmdlist *clist)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
		printf("str :%s\n", clist->str);
		printf("quot:%s\n", clist->quot);
	clist->quot = get_deleted_endflag(clist, '1');
	clist->quot = get_deleted_endflag(clist, '2');
		printf("quot:%s\n", clist->quot);
	new_str = ft_xcalloc(get_new_strlen(clist->str) + 1, sizeof(char));
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
		clear_quot(clist);//クォーテーションを消して、quotフラグを更新
		//クォートで囲まれていないスペースがあったらリストを追加
		;
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
		// serch_env_iolist(data->iolst, data->elst);
		data = data->next;
	}
	return (data);
}