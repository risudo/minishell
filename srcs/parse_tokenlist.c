#include "../includes/minishell.h"

void	expand_status(t_token *list, char *doll_ptr, unsigned char *status)
{
	char	*front;
	char	*back;
	char	*value;

	front = ft_xsubstr(list->str, 0, doll_ptr - list->str);
	back = ft_xsubstr(doll_ptr, 2, ft_strlen(doll_ptr + 2));
	value = ft_xitoa(*status);
	xfree(list->str);
	list->str = ft_strjoin_three(front, value, back);
	xfree(front), xfree(back), xfree(value);
}

bool	is_in_squot(char *str, char *doll_ptr)
{
	int		flag;
	size_t	i;

	i = 0;
	flag = 0;
	while (str[i])
	{
		if (flag == 0 && str[i] == '\'')
			flag = 1;
		else if (flag == 1 && str[i] == '\'')
			flag = 0;
		if (str + i == doll_ptr && flag == 1)
			return (true);
		i++;
	}
	return (false);
}

int	check_token_syntax(t_token *head, t_token *last, unsigned char *status)
{
	int	ret;

	ret = 0;
	if (last->special == PIPE || head->special == PIPE)
	{
		*status = (unsigned char)258;
		put_syntax_error("|");
		ret = -1;
	}
	if (last->special >= IN_REDIRECT && last->special <= OUT_HERE_DOC)
	{
		*status = (unsigned char)258;
		put_syntax_error(last->str);
		ret = -1;
	}
	return (ret);
}

int	parse_tokenlist(t_token *list, unsigned char *status)
{
	t_token	*head;
	t_token	*prev;
	char	*doll_ptr;

	head = list;
	while (list)
	{
		if (is_consecutive_redirect(list))
			list = join_redirect(list);
		doll_ptr = ft_strnstr(list->str, "$?", ft_strlen("list->str"));
		if (doll_ptr && !is_in_squot(list->str, doll_ptr))
		{
			expand_status(list, doll_ptr, status);
			continue ;
		}
		prev = list;
		list = list->next;
	}
	set_special_c(head);
	return (check_token_syntax(head, prev, status));
}
