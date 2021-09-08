#include "minishell.h"
#include "signal.h"

unsigned char	g_status = 0;

void	handler(int signo)
{
	(void)signo;
	g_status = 128 + SIGINT;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

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
			break ;
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
	if (signal(SIGINT, handler) == SIG_ERR
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
