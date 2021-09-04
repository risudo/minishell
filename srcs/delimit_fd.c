#include "minishell.h"

static t_iolist	*insert_redirect_iolist(t_iolist *cur,
			char *redirect, t_special_c type)
{
	t_iolist	*new;

	new = (t_iolist *)ft_xcalloc(1, sizeof(*new));
	new->str = redirect;
	new->quot = get_quot_flag(new->str);
	new->here_doc_fd = -1;
	new->c_type = type;
	cur->next = new;
	return (new);
}

t_iolist	*delimit_fd(t_iolist *cur)
{
	size_t		i;
	char		*fd;
	char		*redirect;
	t_special_c	type;

	i = 0;
	while (ft_isdigit(cur->str[i]))
		i++;
	fd = ft_xsubstr(cur->str, 0, i);
	redirect = ft_xsubstr(cur->str, i, ft_strlen(cur->str + i));
	free(cur->quot), free(cur->str);
	cur->str = fd;
	cur->quot = get_quot_flag(cur->str);
	type = cur->c_type;
	cur->c_type = FD;
	return (insert_redirect_iolist(cur, redirect, type));
}
