#include "../includes/parse.h"

t_execdata	*expansion(t_execdata *data)
{
	t_execdata	*head;

	head = data;
	while (data)
	{
		serch_env_cmdlist(data->clst, data->elst);
		serch_env_iolist(data->iolst, data->elst);
		data = data->next;
	}
	return (data);
}

t_execdata	*create_error_execdata(t_token *tokenlist, unsigned char *status)
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

	tokenlist = tokenize_cmd_by_space(command, status);
	if (*status != 0)
		return (create_error_execdata(tokenlist, status));
	split_operater(tokenlist);
	if (parse_tokenlist(tokenlist, status) == -1)
		return (create_error_execdata(tokenlist, status));
	data = create_execdata(tokenlist, envlist, status);
	clear_tokenlist(tokenlist);
	expansion(data);
	return (check_syntax(data));
}
