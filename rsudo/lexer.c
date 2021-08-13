#include "lexer.h"

bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r'
		|| c == '\n' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

// get str from start to cmd
// return : str

char	*get_str(char *start, char *cmd)
{
	int		i;
	int		len_str;
	char	*str;

	i = 0;
	len_str = cmd - start;
	str = (char *)malloc(sizeof(char) * (len_str + 1));
	if (str == NULL)
		return (NULL);
	while (i < len_str)
	{
		str[i] = start[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

// create a new list
// use calloc not to set tok->next = NULL
// return : new list

t_token	*new_token(t_token *cur, char **cmd, char **start, int *flag_quot)
{
	t_token				*tok;
	static enum e_type	cur_type;

	tok = ft_calloc(1, sizeof(*tok));
	if (tok == NULL)
		return (NULL);
	tok->str = get_str(*start, *cmd);
	if (tok->str == NULL)
		return (NULL);
	tok->type = cur_type;
	cur->next = tok;
	tok->prev = cur;
	while (ft_isspace(**cmd) && *flag_quot == DEFAULT_QUOT)
		(*cmd)++;
	*start = *cmd;
	*flag_quot = DEFAULT_QUOT;
	return (tok);
}

// split by ' ' if it is out of quot.
// return : t_token list that has splited str.

t_token	*tokenize_cmd_by_space(char *cmd)
{
	t_token	head;
	t_token	*cur;
	char	*start;
	int		flag_quot;

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
		if (cur == NULL)
			;//TODO:リストをクリアしてexit
		cmd++;
	}
	if (cmd != start)
		cur = new_token(cur, &cmd, &start, &flag_quot);
	return (head.next);
}

t_token	*insert_new_token(t_token *list, char *ope)
{
	t_token	*new;
	t_token	*next;

	new = ft_calloc(1, sizeof(*list));
	if (new == NULL)
		return (NULL);
	new->str = ope;
	next = list->next;
	list->next = new;
	new->prev = list;
	new->next = next;
	return (new);
}

int	get_newstr_len(t_token *list)
{
	int	i;

	i = 0;
	while (list->str[i] != '>' && list->str[i] != '<' && list->str[i] != '|')
		i++;
	if (i == 0)
		i = 1;
	return (i);
}

// allocate new str removed operater.
// insert new token that has remaining str

t_token	*get_newstr_list(t_token *list)
{
	char	*newstr;
	char	*excluded;
	int		newstr_len;
	int		excluded_i;
	int		newstr_i;

	newstr_i = -1;
	excluded_i = 0;
	newstr_len = get_newstr_len(list);
	newstr = (char *)malloc(sizeof(char) * (newstr_len + 1));
	excluded = (char *)malloc(sizeof(char)
		* (ft_strlen(list->str) - newstr_len + 1));
	if (newstr == NULL || excluded == NULL)
		return (NULL);
	while (++newstr_i < newstr_len)
		newstr[newstr_i] = list->str[newstr_i];
	newstr[newstr_i] = '\0';
	while (list->str[newstr_i])
		excluded[excluded_i++] = list->str[newstr_i++];
	excluded[excluded_i] = '\0';
	free(list->str);//* free
	list->str = newstr;
	list = insert_new_token(list, excluded);
	return (list);
}

// split operater like pipe or ridirect

t_token	*split_operater(t_token *list)
{
	t_token	*head;

	head = list;
	while (list)
	{
		if (ft_strlen(list->str) >= 2 && (ft_strchr(list->str, '>')
				|| ft_strchr(list->str, '<') || ft_strchr(list->str, '|')))
		{
			list = get_newstr_list(list);
			if (list == NULL)
				;//TODO: リストをクリアしてexit
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

	list = tokenize_cmd_by_space(command);
	// put_list(list);
	list = split_operater(list);
	put_list(list);
}

int	main(void)
{
	char	*cmd1 = "echo   'test hoge'  'fuga' |   cat|cat>>file";
	char	*cmd2 = "echo test | cat";

	printf("cmd: [%s]\n", cmd1);
	lexer(cmd1);
}
