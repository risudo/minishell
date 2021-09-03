#include "minishell.h"

/*
** Tokenize command and create t_token list.
*/

bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

static t_token	*new_token(t_token *cur, char **cmd,
			 char **start, t_quottype *flag_quot)
{
	t_token	*tok;
	size_t	len;

	tok = (t_token *)ft_xcalloc(1, sizeof(*tok));
	len = *cmd - *start;
	tok->str = ft_xsubstr(*start, 0, len);
	tok->type = *flag_quot;
	cur->next = tok;
	tok->prev = cur;
	if (*flag_quot == D_QUOT || *flag_quot == S_QUOT)
	{
		put_syntax_error(tok->str);
		return (NULL);
	}
	while (ft_isspace(**cmd))
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

t_token	*tokenize_cmd_by_space(char *cmd, unsigned char *status, bool *err)
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
			cur = new_token(cur, &cmd, &start, &flag_quot), cmd--;
		flag_quot = get_flag_quot(cmd, flag_quot), cmd++;
	}
	if (cur != NULL && cmd != start)
		cur = new_token(cur, &cmd, &start, &flag_quot);
	if (cur == NULL)
	{
		*status = (unsigned char)258;
		*err = true;
		return (head.next);
	}
	cur = head.next;
	cur->prev = NULL;
	return (cur);
}
