#include "minishell.h"

void	xwaitpid(pid_t pid, int *wstatus, int options)
{
	if (waitpid(pid, wstatus, options) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}

void	xpipe(int *pipefd, t_execdata *data)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	open_fd_handler(data, OPEN, pipefd[READ]);
	open_fd_handler(data, OPEN, pipefd[WRITE]);
}

pid_t	xfork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}
