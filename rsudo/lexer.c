#include "lexer.h"

bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r'
		|| c == '\n' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

char	*get_str(char *start, char *cmd)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (start[i] != *cmd)
		i++;
	str = (char *)malloc(sizeof(char) * (i + 1));//!malloc
	i = 0;
	j = cmd - start;
	while (j--)
	{
		str[i] = start[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

t_token	*new_token(t_token *cur, char **cmd, char **start, int *flag_quot)
{
	t_token				*tok;
	static enum e_type	cur_type;

	tok = ft_calloc(1, sizeof(*tok));//!malloc
	tok->str = get_str(*start, *cmd);
	// cur_type = get_type(cur_type);
	tok->type = cur_type;
	cur->next = tok;
	tok->prev = cur;
		// printf("*flag_quot: %d\n", *flag_quot);
	while (ft_isspace(**cmd) && *flag_quot == DEFAULT_QUOT)
		(*cmd)++;
	*start = *cmd;
	*flag_quot = DEFAULT_QUOT;
	return (tok);
}
//setが来たら区切ってlist->cmdに入れる
t_token	*tokenize_space(char *cmd)
{
	t_token	head;
	t_token	*cur;
	char	*start;
	int	flag_quot;

	flag_quot = DEFAULT_QUOT;
	cur = &head;
	start = cmd;
	while (*cmd)
	{
		if ((*cmd == '\"' || *cmd == '\'') && flag_quot == START_QUOT)
			flag_quot = END_QUOT;
		if ((*start == '\"' || *start == '\'') && flag_quot == DEFAULT_QUOT)
			flag_quot = START_QUOT;
		if (*cmd == ' ' && flag_quot > 0)
			cur = new_token(cur, &cmd, &start, &flag_quot);
		cmd++;
	}
	if (cmd != start)
		cur = new_token(cur, &cmd, &start, &flag_quot);
	return (head.next);
}

t_token	*token_insert(t_token *list, char *ope)
{
	t_token	*new;
	t_token	*next;

	new = ft_calloc(1, sizeof(*list)); //!malloc
	new->str = ope;
	next = list->next;
	list->next = new;
	new->prev = list;
	new->next = next;
	return (new);
}

t_token	*new_token_ope(t_token *list, int i)
{
	char	*str;
	char	*ope;
	int		j;

	j = 0;
	str = (char *)malloc(sizeof(char) * (i + 1)); //!malloc
	ope = (char *)malloc(sizeof(char) * (ft_strlen(list->str) - i + 1)); //!malloc
	while (i--)
	{
		str[j] = list->str[j];
		j++;
	}
	str[j] = '\0';
	i++;
	while (list->str[j])
		ope[i++] = list->str[j++];
	ope[i] = '\0';
	free(list->str);//* free
	list->str = str;
	list = token_insert(list, ope);
	return (list);
}

t_token	*pick_ope(t_token *list)
{
	int	i;

	i = 0;
	while (list->str[i] != '>' && list->str[i] != '<' && list->str[i] != '|')
	{
		i++;
	}
	if (i == 0)
		return (new_token_ope(list, 1));
	else
		return (new_token_ope(list, i));
}

t_token	*tokenize_ope(t_token *list)
{
	int		i;
	t_token	*head;

	head = list;
	while (list)
	{
		//パイプ、リダイレクトがあったら分割
		if (ft_strlen(list->str) >= 2
			&& (ft_strchr(list->str, '>') || ft_strchr(list->str, '<') || ft_strchr(list->str, '|')))
		{
			list = pick_ope(list);
			continue ;
		}
		list = list->next;
	}
	return (head);
}

//単語に分割してリストに入れていく
void	lexer(char *command)
{
	t_token	*list;

	list = tokenize_space(command);
	put_list(list);
	list = tokenize_ope(list);
	put_list(list);
}

int	main()
{
	char *cmd1 = "echo   'test hoge'  'fuga' |   cat|cat>>file";
	char *cmd2 = "echo test | cat";

	printf("cmd: [%s]\n", cmd1);
	lexer(cmd1);
}