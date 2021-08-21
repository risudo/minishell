#include "../includes/parse.h"

int	parse_tokenlist(t_token *list)
{
	t_token	*head;
	t_token	*prev;

	head = list;
	while (list)
	{
		if (is_space_after_quot(list))
			list = trim_list(list);
		if (is_consecutive_redirect(list))
			list = join_redirect(list);
		prev = list;
		list = list->next;
	}
	set_special_c(head);
	if (prev->special == PIPE)
	{
		clear_tokenlist(head);
		ft_putendl_fd("Error: ", STDERR_FILENO);
		return (-1);
	}
	else
		return (0);
}

t_execdata	*parse_cmd(char *command, char **envp)
{
	t_token		*tokenlist;
	t_execdata	*data;
	t_envlist	*envlist;

	tokenlist = tokenize_cmd_by_space(command);
	if (tokenlist == NULL)
		return (NULL);
	if (split_operater(tokenlist) == -1 || parse_tokenlist(tokenlist) == -1)
		return (NULL);
	put_tokenlist(tokenlist);
	envlist = create_envlist(envp);
	data = create_execdata(tokenlist, envlist);
	clear_tokenlist(tokenlist);
	return (data);
}
