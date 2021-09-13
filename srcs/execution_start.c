#include "minishell.h"

/*
** All execution starts with execute_start().
** If there are multiple executions (been separated by '|'),
** execute_loop() runs each execution.
*/

static void	child_execute(t_execdata *data, int prev_pipe_read, \
						int piperead, int pipewrite)
{
	if (ft_dup2(prev_pipe_read, STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	xclose(piperead);
	if (data->next && ft_dup2(pipewrite, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	else
		xclose(pipewrite);
	if (setdata_cmdline_redirect(data) != -1)
		execute_command(data);
	exit(g_status);
}

static int	parent_connect_fd(t_execdata *data, int prev_pipe_read, \
						int piperead, int pipewrite)
{
	xclose(pipewrite);
	if (prev_pipe_read != STDIN_FILENO)
		xclose(prev_pipe_read);
	if (data->next == NULL)
		xclose(piperead);
	return (piperead);
}

/*
** Loop flow
** 1 pipe() to create pipe and prepare pipe_fd
** 2 fork() to create child process
** 3 parent process update prev_pipe_read with (current)pipe_fd
** 4 child process
**   set prev_pipe_read -> input fd
**   set (current)pipe_fd -> output fd
**   set data
**   run execution
*/
static void	execute_loop(t_execdata *data, int *wstatus)
{
	int	pid;
	int	pipefd[PIPEFD_NUM];
	int	prev_pipe_read;

	xsignal(SIGINT, SIG_IGN);
	prev_pipe_read = STDIN_FILENO;
	while (data)
	{
		ft_pipe(pipefd);
		pid = xfork();
		if (pid == 0)
		{
			xsignal(SIGINT, SIG_DFL);
			xsignal(SIGQUIT, SIG_DFL);
			child_execute(data, prev_pipe_read, \
					pipefd[READ], pipefd[WRITE]);
		}
		else
			prev_pipe_read = parent_connect_fd(data, \
					prev_pipe_read, pipefd[READ], pipefd[WRITE]);
		data = data->next;
	}
	xwaitpid(pid, wstatus, 0);
	xsignal(SIGINT, signal_handler);
}

/*
** SAVE: save fd (point to standard I/O)
** RESTORE: set STD*_FILENO to point to standard I/O
*/
static int	std_fd_handler(t_execdata *data, t_fd_mode mode)
{
	if (mode == SAVE)
	{
		if (ft_dup(data, ORIGINAL_IN, STDIN_FILENO) == -1 || \
			ft_dup(data, ORIGINAL_OUT, STDOUT_FILENO) == -1 || \
			ft_dup(data, ORIGINAL_ERR, STDERR_FILENO) == -1)
		{
			g_status = 1;
			return (-1);
		}
	}
	else if (mode == RESTORE)
	{
		if (ft_dup2(data->stdfd[ORIGINAL_IN], STDIN_FILENO) == -1 || \
			ft_dup2(data->stdfd[ORIGINAL_OUT], STDOUT_FILENO) == -1 || \
			ft_dup2(data->stdfd[ORIGINAL_ERR], STDERR_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	return (0);
}

/*
** If the execution is a single and it's built-in command,
** handle standard I/O and run execution.
** If there are multiple executions,
** run execute_loop() and get last exit status.
*/
void	execute_start(t_execdata *data)
{
	int	last_child_status;

	if (setdata_heredoc_cmdtype(data) == -1)
		return ;
	if (data->next == NULL && data->cmd_type != OTHER)
	{
		if (std_fd_handler(data, SAVE) != -1 && \
				setdata_cmdline_redirect(data) != -1)
			execute_command(data);
		std_fd_handler(data, RESTORE);
	}
	else
	{
		execute_loop(data, &last_child_status);
		set_status_from_child_status(last_child_status);
		while (data->next)
			wait(NULL), data = data->next;
	}
}
