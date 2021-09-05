#include "minishell.h"

/*
** All execution starts with execute_start().
** If there are multiple executions (been separated by '|'),
** execute_loop() runs each execution.
*/

static void	child_execute(t_execdata *data, int prev_pipe_read, \
						int piperead, int pipewrite)
{
	ft_dup2(prev_pipe_read, STDIN_FILENO);
	xclose(piperead);
	if (data->next)
		ft_dup2(pipewrite, STDOUT_FILENO);
	if (setdata_cmdline_redirect(data) == 0)
		execute_command(data);
	exit(*(data->status));
}

static int	parent_connect_fd(t_execdata *data, int prev_pipe_read, \
						int piperead, int pipewrite)
{
	xclose(pipewrite);
	if (prev_pipe_read != STDIN_FILENO)
		xclose(prev_pipe_read);
	if (!data->next)
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
int	execute_loop(t_execdata *data)
{
	int	pid;
	int	pipefd[PIPEFD_NUM];
	int	prev_pipe_read;

	prev_pipe_read = STDIN_FILENO;
	while (data)
	{
		xpipe(pipefd);
		pid = xfork();
		if (pid == 0)
			child_execute(data, prev_pipe_read, \
					pipefd[READ], pipefd[WRITE]);
		else
			prev_pipe_read = parent_connect_fd(data, \
					prev_pipe_read, pipefd[READ], pipefd[WRITE]);
		data = data->next;
	}
	return (pid);
}

static void	std_fd_handler(t_execdata *data, int mode)
{
	if (mode == STD_SAVE)
	{
		data->ori_stdin = xdup(STDIN_FILENO);
		data->ori_stdout = xdup(STDOUT_FILENO);
		data->ori_stderr = xdup(STDERR_FILENO);
	}
	else if (mode == STD_RESTORE)
	{
		ft_dup2(data->ori_stdin, STDIN_FILENO);
		ft_dup2(data->ori_stdout, STDOUT_FILENO);
		ft_dup2(data->ori_stderr, STDERR_FILENO);
		open_fd_handler(data, ALL_CLOSE, 0);
	}
}

/*
** If the execution is a single and it's built-in command,
** handle standard I/O and run execution.
** If there are multiple executions,
** run execute_loop() and get last exit status.
*/
void	execute_start(t_execdata *data)
{
	int			lastchild_pid;
	int			wstatus;

	setdata_heredoc_cmdtype(data);
	if (data->next == NULL && data->cmd_type != OTHER)
	{
		std_fd_handler(data, STD_SAVE);
		if (setdata_cmdline_redirect(data) == 0)
			execute_command(data);
		free_2d_array(data->cmdline);
		std_fd_handler(data, STD_RESTORE);
	}
	else
	{
		lastchild_pid = execute_loop(data);
		xwaitpid(lastchild_pid, &wstatus, 0);
		*(data->status) = WEXITSTATUS(wstatus);
		while (data->next)
		{
			xwaitpid(0, NULL, 0);
			data = data->next;
		}
	}
}
