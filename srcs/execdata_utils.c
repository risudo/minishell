#include "../includes/parse.h"

char	get_flag(char c, char flag)
{
	if ((c == '\"' && (flag == '0' || flag == '1' || flag == '2')) || (flag == 'D' && c != '\"'))
		return ('D');
	else if ((c == '\'' && (flag == '0' || flag == '1' || flag == '2')) || (flag == 'S' && c != '\''))
		return ('S');
	else if (c == '\"' && flag == 'D')
		return ('2');
	else if (c == '\'' && flag == 'S')
		return ('1');
	else
		return ('0');
}

char	*get_quot_flag(char *str)
{
	char	*ret;
	char	flag;
	size_t	i;

	ret = (char *)ft_xcalloc(ft_strlen(str) + 1, sizeof(char));
	i = 0;
	flag = '0';
	while (*str)
	{
		flag = get_flag(*str, flag);
		ret[i++] = flag;
		str++;
	}
	return (ret);
}

static t_cmdlist	*new_clst(t_cmdlist *cur, t_token *token)
{
	t_cmdlist	*new;

	new = (t_cmdlist *)ft_xcalloc(1, sizeof(*new));
	cur->next = new;
	new->str = ft_xstrdup(token->str);
	new->quot = get_quot_flag(new->str);
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


static t_iolist	*new_iolst(t_iolist *cur, t_token *token)
{
	t_iolist	*new;

	new = (t_iolist *)ft_xcalloc(1, sizeof(*new));
	new->here_doc_fd = -1;
	new->c_type = token->special;
	new->here_doc_fd = -1;
	cur->next = new;
	new->str = ft_xstrdup(token->str);
	new->quot = get_quot_flag(new->str);
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
	return (head.next);
}
