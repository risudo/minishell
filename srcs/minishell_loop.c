#include "minishell.h"

void	minishell_loop(char **envp)
{
	t_envlist		*elst;
	t_execdata		*data;
	char			*line;

	elst = create_envlist(envp);
	while (1)
	{
		line = readline("\033[32mminishell > \033[39m");
		if (!line)
			exit(1);
		if (line[0] != '\0')
		{
			data = parse_cmd(line, elst);
			execute_start(data);
			clear_execdata(data);
			add_history(line);
		}
		free(line);
	}
}
