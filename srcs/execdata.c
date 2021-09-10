#include "minishell.h"

/*
** Split tokenlist by pipe, and create execdata list.
** execdata has cmdlist, iolist, and envlist.
** iolist has the token related redirect.
** cmdlist has other token.
*/

static t_iolist	*get_iolst(t_token *start, t_token *cur_token)
{
	t_iolist	head;
	t_iolist	*cur;

	head.next = NULL;
	cur = &head;
	while (start != cur_token)
	{
		if (!is_cmd(start))
		{
			cur = new_iolst(cur, start);
			if (cur->c_type >= IN_REDIRECT && cur->c_type <= OUT_REDIRECT
				&& ft_isdigit(cur->str[0]))
			{
				cur = delimit_fd(cur);
			}
		}
		start = start->next;
	}
	return (head.next);
}

static t_cmdlist	*get_clst(t_token *start, t_token *cur_token)
{
	t_cmdlist	head;
	t_cmdlist	*cur;

	head.next = NULL;
	cur = &head;
	while (start != cur_token)
	{
		if (is_cmd(start))
		{
			cur = new_clst(cur, start);
		}
		start = start->next;
	}
	return (head.next);
}

static t_execdata	*new_execdata(t_execdata *cur, t_token *start,
		t_token *cur_token, t_envlist *envlist)
{
	t_execdata	*new;

	new = (t_execdata *)ft_xcalloc(1, sizeof(*new));
	new->cmdline = NULL;
	new->stdfd[ORIGINAL_IN] = -42;
	new->stdfd[ORIGINAL_OUT] = -42;
	new->stdfd[ORIGINAL_ERR] = -42;
	new->clst = get_clst(start, cur_token);
	new->iolst = get_iolst(start, cur_token);
	new->elst = envlist;
	cur->next = new;
	return (new);
}

/*
** @(tokenlist)
**  command list splited by space or other operater like pipe.
** @(envlist)
**  list that has environment variable's key and value
** Create execdata from token list. Return the execdata.
*/

t_execdata	*create_execdata(t_token *tokenlist, t_envlist *envlist)
{
	t_execdata	head;
	t_execdata	*cur;
	t_token		*start;

	cur = &head;
	start = tokenlist;
	while (tokenlist)
	{
		if (tokenlist->special == PIPE)
		{
			cur = new_execdata(cur, start, tokenlist, envlist);
			start = tokenlist->next;
		}
		tokenlist = tokenlist->next;
	}
	cur = new_execdata(cur, start, tokenlist, envlist);
	return (head.next);
}
