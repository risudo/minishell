#include "minishell.h"

void	minishell_loop(char **envp)
{
	unsigned char	*status;
	t_envlist		*elst;
	t_execdata		*data;
	char			*line;

	status = (unsigned char *)ft_calloc(1, sizeof(unsigned char));
	elst = create_envlist(envp);
	while (1)
	{
		line = readline("\033[32mminishell > \033[39m");
		if (!line)
			exit(1);
		if (line[0] != '\0')
		{
			data = parse_cmd(line, elst);
			*status = g_status;
			data->status = status;
			execute_start(data);
			clear_execdata(data);
			add_history(line);
		}
		free(line);
	}
}
