#include "minishell.h"
#include "signal.h"

void	handler(int signo)
{
	(void)signo;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	minishell_loop(envp);
	(void)argc;
	(void)argv;
}
