#include "../includes/parse.h"

static t_cmdlist	*new_clst(t_cmdlist *cur, t_token *token)
{
	t_cmdlist	*new;

	new = ft_xcalloc(1, sizeof(*new));
	cur->next = new;
	new->prev = cur;
	new->str = ft_xstrdup(token->str);
	new->quot = token->type;
	return (new);
}

static bool	is_cmd(t_token *start)
{
	if (start->special == ELSE
		&& (start->prev == NULL
			|| start->prev->special == ELSE
			|| start->prev->special == PIPE))
		return (true);
	else
		return (false);
}

t_cmdlist	*get_clst(t_token *start, t_token *cur_token)
{
	t_cmdlist	head;
	t_cmdlist	*cur;

	cur = &head;
	if (start == cur_token)
		;
	while (start != cur_token)
	{
		if (is_cmd(start))
		{
			cur = new_clst(cur, start);
		}
		start = start->next;
	}
	cur = head.next;
	cur->prev = NULL;
	return (cur);
}

static t_iolist	*new_iolst(t_iolist *cur, t_token *token)
{
	t_iolist	*new;

	new = ft_xcalloc(1, sizeof(*new));
	new->here_doc_fd = -1;
	new->c_type = token->special;
	new->here_doc_fd = -1;
	cur->next = new;
	new->prev = cur;
	new->str = ft_xstrdup(token->str);
	new->quot = token->type;
	return (new);
}

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
	cur = head.next;
	return (cur);
}
