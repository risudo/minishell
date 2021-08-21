#include "../includes/parse.h"

static bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

// create a new list
// use xcalloc not to set tok->next = NULL
// return : new list

static t_token	*new_token(t_token *cur, char **cmd,
			 char **start, t_quottype *flag_quot)
{
	t_token	*tok;
	int		len;

	if (*flag_quot == D_QUOT || *flag_quot == S_QUOT)
	{
		ft_putendl_fd("Error: Quotes are not closed", STDERR_FILENO);
		return (NULL);
	}
	tok = ft_xcalloc(1, sizeof(*tok));
	len = *cmd - *start;
	tok->str = ft_xsubstr(*start, 0, len);
	tok->type = *flag_quot;
	cur->next = tok;
	tok->prev = cur;
	while (ft_isspace(**cmd) && *flag_quot == DEFAULT)
		(*cmd)++;
	*start = *cmd;
	return (tok);
}

static t_quottype	get_flag_quot(char *cmd, t_quottype flag_quot)
{
	if ((flag_quot == S_QUOT && *cmd == '\''))
		return (END_S_QUOT);
	else if ((flag_quot == D_QUOT && *cmd == '\"'))
		return (END_D_QUOT);
	else if (((flag_quot == DEFAULT || flag_quot == END_S_QUOT
				|| flag_quot == END_D_QUOT) && *cmd == '\'')
		|| flag_quot == S_QUOT)
		return (S_QUOT);
	else if (((flag_quot == DEFAULT || flag_quot == END_D_QUOT
				|| flag_quot == END_S_QUOT) && *cmd == '\"')
		|| flag_quot == D_QUOT)
		return (D_QUOT);
	else
		return (DEFAULT);
}

static char	*skip_space(char **cmd)
{
	while (ft_isspace(**cmd))
		(*cmd)++;
	return (*cmd);
}
// split by space if it is out of quot.
// return : t_token list that has splited str.

t_token	*tokenize_cmd_by_space(char *cmd)
{
	t_token		head;
	t_token		*cur;
	char		*start;
	t_quottype	flag_quot;

	flag_quot = DEFAULT;
	cur = &head;
	start = skip_space(&cmd);
	while (*cmd)
	{
		if (ft_isspace(*cmd) && !(flag_quot == D_QUOT || flag_quot == S_QUOT))
			cur = new_token(cur, &cmd, &start, &flag_quot);
		flag_quot = get_flag_quot(cmd, flag_quot), cmd++;
	}
	if (cur != NULL && cmd != start)
		cur = new_token(cur, &cmd, &start, &flag_quot);
	if (cur == NULL)
	{
		clear_tokenlist(head.next);
		return (NULL);
	}
	cur = head.next;
	cur->prev = NULL;
	return (cur);
}
