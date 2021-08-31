#include "minishell.h"
#include "signal.h"

void	handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	signal(SIGINT, handler);
	minishell_loop(envp);
	(void)argc;
	(void)argv;
}
