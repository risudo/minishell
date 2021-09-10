#include "minishell.h"

unsigned char	g_status = 0;

void	minishell_loop(char **envp)
{
	t_envlist		*elst;
	t_execdata		*data;
	char			*line;

	elst = create_envlist(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			clear_envlist(elst);
			break ;
		}
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

int	main(int argc, char **argv, char **envp)
{
	if (signal(SIGINT, signal_handler) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	minishell_loop(envp);
	(void)argc;
	(void)argv;
	return (g_status);
}
