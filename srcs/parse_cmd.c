#include "minishell.h"

static t_execdata	*expand_variable(t_execdata *data)
{
	while (data)
	{
		expand_cmdlist(&data->clst, data->elst);
		data = data->next;
	}
	return (data);
}

static t_execdata	*create_error_execdata(t_token *tokenlist)
{
	t_execdata	*data;

	data = (t_execdata *)ft_calloc(1, sizeof(*data));
	clear_tokenlist(tokenlist);
	return (data);
}

t_execdata	*parse_cmd(char *command, t_envlist *envlist)
{
	t_token		*tokenlist;
	t_execdata	*data;
	bool		err;

	err = false;
	tokenlist = tokenize_cmd(command, &err);
	if (err == true)
		return (create_error_execdata(tokenlist));
	split_operater(tokenlist);
	if (parse_tokenlist(tokenlist) == -1)
		return (create_error_execdata(tokenlist));
	data = create_execdata(tokenlist, envlist);
	clear_tokenlist(tokenlist);
	expand_variable(data);
	return (data);
}
