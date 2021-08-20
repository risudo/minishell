#include "lexer.h"

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
	envlist = create_envlist(envp);
	data = create_execdata(tokenlist, envlist);
	clear_tokenlist(tokenlist);
	return (data);
}
