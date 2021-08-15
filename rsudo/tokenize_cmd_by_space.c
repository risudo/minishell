#include "lexer.h"

static bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

// get str from start to cmd
// return : str

static char	*get_str(char *start, char *cmd)
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

static t_token	*new_token(t_token *cur, char **cmd,
			 char **start, t_quottype *flag_quot)
{
	t_token				*tok;

	tok = ft_calloc(1, sizeof(*tok));
	if (tok == NULL)
		return (NULL);
	tok->str = get_str(*start, *cmd);
	if (tok->str == NULL)
		return (NULL);
	tok->type = *flag_quot;
	cur->next = tok;
	tok->prev = cur;
	while (ft_isspace(**cmd) && *flag_quot == DEFAULT)
		(*cmd)++;
	*start = *cmd;
	return (tok);
}

t_quottype	get_flag_quot(char *cmd, t_quottype flag_quot)
{
	if ((flag_quot == S_QUOT && *cmd == '\''))
		return (END_S_QUOT);
	else if ((flag_quot == D_QUOT && *cmd == '\"'))
		return (END_D_QUOT);
	else if ((flag_quot == DEFAULT && *cmd == '\'') || flag_quot == S_QUOT)
		return (S_QUOT);
	else if ((flag_quot == DEFAULT && *cmd == '\"') || flag_quot == D_QUOT)
		return (D_QUOT);
	else
		return (DEFAULT);
}

// split by ' ' if it is out of quot.
// return : t_token list that has splited str.

t_token	*tokenize_cmd_by_space(char *cmd)
{
	t_token		head;
	t_token		*cur;
	char		*start;
	t_quottype	flag_quot;

	flag_quot = DEFAULT;
	cur = &head;
	while (ft_isspace(*cmd))
		cmd++;
	start = cmd;
	while (*cmd)
	{
		if (ft_isspace(*cmd) && (flag_quot == DEFAULT
				|| flag_quot == END_S_QUOT || flag_quot == END_D_QUOT))
			cur = new_token(cur, &cmd, &start, &flag_quot);
		if (cur == NULL)
			clear_tokenlist(head.next);
		flag_quot = get_flag_quot(cmd, flag_quot);
		cmd++;
	}
	if (cmd != start)
		cur = new_token(cur, &cmd, &start, &flag_quot);
	return (head.next);
}
