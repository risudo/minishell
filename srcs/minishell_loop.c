#include "minishell.h"

void	minishell_loop(char **envp)
{
	unsigned char	*status;
	t_envlist		*elst;
	t_execdata		*data;
	char			*line;

	status = (unsigned char *)malloc(sizeof(unsigned char));
	*status = 0;
	elst = create_envlist(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			exit(1);
		if (line[0] != '\0')
		{
			data = parse_cmd(line, elst, status);
			execute_start(data);
			clear_execdata(data);
			add_history(line);
		}
		free(line);
	}
}