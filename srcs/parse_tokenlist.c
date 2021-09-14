#include "minishell.h"

/*
** Parse token list do following
** 1 If the character of redirects are consecutive, join these.
** 2 Expand "$?".
** 3 Check syntax error.
*/

void	put_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ",
		 STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

static void	expand_status(t_token *list, char *doll_ptr)
{
	char	*front;
	char	*back;
	char	*value;

	front = ft_xsubstr(list->str, 0, doll_ptr - list->str);
	back = ft_xsubstr(doll_ptr, 2, ft_strlen(doll_ptr + 2));
	value = ft_xitoa(g_status);
	free(list->str);
	list->str = ft_strjoin_three(front, value, back);
	free(front), free(back), free(value);
}

static bool	is_in_squot(char *str, char *doll_ptr)
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

static int	check_token_syntax(t_token *head, t_token *last)
{
	int	ret;

	ret = 0;
	if (last && (last->special == PIPE || head->special == PIPE))
		put_syntax_error("|"), ret = -1;
	if (last && last->special >= IN_REDIRECT && last->special <= OUT_HERE_DOC)
		put_syntax_error(last->str), ret = -1;
	while (head && ret == 0)
	{
		if (head->special >= IN_REDIRECT && head->special <= OUT_HERE_DOC
			&& ft_strlen(head->str) >= 3)
		{
			ret = -1;
			put_syntax_error(head->str);
		}
		head = head->next;
	}
	if (ret == -1)
		g_status = 2;
	return (ret);
}

int	parse_tokenlist(t_token *list)
{
	t_token	*head;
	t_token	*prev;
	char	*doll_ptr;

	head = list;
	prev = NULL;
	while (list)
	{
		doll_ptr = ft_strnstr(list->str, "$?", ft_strlen("list->str"));
		if (doll_ptr && !is_in_squot(list->str, doll_ptr))
		{
			expand_status(list, doll_ptr);
			continue ;
		}
		prev = list;
		list = list->next;
	}
	set_special_c(head);
	return (check_token_syntax(head, prev));
}
