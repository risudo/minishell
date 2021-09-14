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
			line = ft_xstrdup("exit");
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
	xsignal(SIGINT, signal_handler);
	xsignal(SIGQUIT, SIG_IGN);
	minishell_loop(envp);
	(void)argc;
	(void)argv;
	return (g_status);
}
