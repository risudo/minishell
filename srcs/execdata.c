#include "../includes/parse.h"

t_execdata	*new_execdata(t_execdata *cur, t_token *start,
		t_token *cur_token, t_execarg *arg)
{
	t_execdata	*new;

	new = ft_xcalloc(1, sizeof(*new));
	new->cmdline = NULL;
	new->in_fd = STDIN_FILENO;
	new->out_fd = STDOUT_FILENO;
	new->clst = get_clst(start, cur_token);
	new->iolst = get_iolst(start, cur_token);
	new->elst = arg->envlist;
	new->status = arg->status;
	cur->next = new;
	return (new);
}

t_execdata	*create_execdata(t_token *tokenlist, t_envlist *envlist)
{
	t_execdata	head;
	t_execdata	*cur;
	t_token		*start;
	t_execarg	arg;

	arg.status = ft_xcalloc(1, sizeof(int));
	arg.envlist = envlist;
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
