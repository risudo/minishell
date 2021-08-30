#include "../includes/parse.h"

/*
** Split tokenlist by pipe, and create execdata list.
** execdata has cmdlist and iolist.
** iolist has the token related redirect.
** cmdlist has other token.
*/

t_iolist	*get_iolst(t_token *start, t_token *cur_token)
{
	t_iolist	head;
	t_iolist	*cur;
	t_special_c	prev;

	head.next = NULL;
	cur = &head;
	while (start != cur_token)
	{
		if (!is_cmd(start))
		{
			cur = new_iolst(cur, start);
		}
		prev = start->special;
		start = start->next;
	}
	return (head.next);
}

t_cmdlist	*get_clst(t_token *start, t_token *cur_token)
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

t_execdata	*new_execdata(t_execdata *cur, t_token *start,
		t_token *cur_token, t_execarg *arg)
{
	t_execdata	*new;

	new = (t_execdata *)ft_xcalloc(1, sizeof(*new));
	new->cmdline = NULL;
	new->clst = get_clst(start, cur_token);
	new->iolst = get_iolst(start, cur_token);
	new->elst = arg->envlist;
	new->status = arg->status;
	cur->next = new;
	return (new);
}

t_execdata	*create_execdata(t_token *tokenlist,
		t_envlist *envlist, unsigned char *status)
{
	t_execdata	head;
	t_execdata	*cur;
	t_token		*start;
	t_execarg	arg;

	arg.envlist = envlist;
	arg.status = status;
	cur = &head;
	start = tokenlist;
	while (tokenlist)
	{
		if (tokenlist->special == PIPE)
		{
			cur = new_execdata(cur, start, tokenlist, &arg);
			start = tokenlist->next;
		}
		tokenlist = tokenlist->next;
	}
	cur = new_execdata(cur, start, tokenlist, &arg);
	return (head.next);
}
