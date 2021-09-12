#include "minishell.h"
#include "signal.h"

unsigned char	g_status = 0;


void end(void)__attribute__((destructor));
void end(void)
{
    pid_t current_pid;
    char cmd_str[50];

    current_pid = getpid();
    sprintf(cmd_str, "%s %d %s\n", "leaks", current_pid, ">> leaks.txt 2>&1");
    int ret = system(cmd_str);
    if (ret) printf("\e[31m!leak detected!\e[0m\n");
}

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
		if (g_status != 0)
			printf("\x1b[31m[%d] \033[m", g_status);
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