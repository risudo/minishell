#include "minishell.h"

static t_execdata	*expand_variable(t_execdata *data)
{
	t_execdata	*head;

	head = data;
	while (data)
	{
		serch_env_cmdlist(data->clst, data->elst);
		data = data->next;
	}
	return (data);
}

static t_execdata	*create_error_execdata(t_token *tokenlist,
		unsigned char *status)
{
	t_execdata	*data;

	data = (t_execdata *)ft_calloc(1, sizeof(*data));
	clear_tokenlist(tokenlist);
	data->status = status;
	return (data);
}

t_execdata	*parse_cmd(char *command, t_envlist *envlist, unsigned char *status)
{
	t_token		*tokenlist;
	t_execdata	*data;
	bool		err;

	err = false;
	tokenlist = tokenize_cmd_by_space(command, status, &err);
	if (err == true)
		return (create_error_execdata(tokenlist, status));
	split_operater(tokenlist);
	if (parse_tokenlist(tokenlist, status) == -1)
		return (create_error_execdata(tokenlist, status));
	data = create_execdata(tokenlist, envlist, status);
	clear_tokenlist(tokenlist);
	expand_variable(data);
	return (data);
}
