#include "minishell.h"

/*
** All execution starts with execute_start().
** If there are multiple executions (been separated by '|'),
** execute_loop() runs each execution.
*/

void	execute_command(t_execdata *data)
{
	void	(*cmd_func[CMD_NUM])(t_execdata *data);

	cmd_func[ECHO] = builtin_echo;
	cmd_func[CD] = builtin_cd;
	cmd_func[PWD] = builtin_pwd;
	cmd_func[EXPORT] = builtin_export;
	cmd_func[UNSET] = builtin_unset;
	cmd_func[ENV] = builtin_env;
	cmd_func[EXIT] = builtin_exit;
	cmd_func[OTHER] = non_builtin;
	cmd_func[NON_CMD] = no_command;
	cmd_func[data->cmd_type](data);
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
		{
			ft_dup2(prev_pipe_read, STDIN_FILENO);
			xclose(pipefd[READ]);
			if (data->next)
				ft_dup2(pipefd[WRITE], STDOUT_FILENO);
			if (setdata_cmdline_redirect(data) == 0)
				execute_command(data);
			exit(*(data->status));
		}
		xclose(pipefd[WRITE]);
		if (prev_pipe_read != STDIN_FILENO)
			xclose(prev_pipe_read);
		prev_pipe_read = pipefd[READ];
		data = data->next;
	}
	xclose(prev_pipe_read);
	return (pid);
}

void	open_fd_handler(t_execdata *data, int mode, int open_fd)
{
	static char	open_fd_flag[FD_MAX + 1];
	int			index_fd;

	if (mode == FD_REDIRECTED && !open_fd_flag[open_fd])
	{
		if(data->ori_stdin == open_fd)
			data->ori_stdin = xdup(open_fd);
		else if (data->ori_stdout == open_fd)
			data->ori_stdout = xdup(open_fd);
		else if (data->ori_stderr == open_fd)
			data->ori_stderr = xdup(open_fd);
		open_fd_flag[open_fd]++;
	}
	else if (mode == CLOSE)
	{
		index_fd = 0;
		while (index_fd <= FD_MAX)
		{
			if (open_fd_flag[index_fd])
				xclose(index_fd);
			open_fd_flag[index_fd] = 0;
			index_fd++;
		}
	}
}

static void	std_fd_handler(t_execdata *data, int mode)
{
	if (mode == SAVE)
	{
		data->ori_stdin = xdup(STDIN_FILENO);
		data->ori_stdout = xdup(STDOUT_FILENO);
		data->ori_stderr = xdup(STDERR_FILENO);
	}
	else if (mode == RESTORE)
	{
		ft_dup2(data->ori_stdin, STDIN_FILENO);
		ft_dup2(data->ori_stdout, STDOUT_FILENO);
		ft_dup2(data->ori_stderr, STDERR_FILENO);
		open_fd_handler(data, CLOSE, 0);
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
		std_fd_handler(data, SAVE);
		if (setdata_cmdline_redirect(data) == 0)
			execute_command(data);
		free_2d_array(data->cmdline);
		std_fd_handler(data, RESTORE);
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
