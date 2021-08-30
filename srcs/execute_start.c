# include "execute.h"

enum stdfd_mode
{
	SAVE,
	RESTORE
};
/*
** all execution starts with execute_start().
** if there are multiple execution separate by '|',
** execute_loop() runs each command.
*/

//execute command according to data->cmd_type
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
	cmd_func[NON_CMD] = non_command;
	cmd_func[data->cmd_type](data);
}

//this loop execute commands and connect those outfputs to next command
int	execute_loop(t_execdata *data)
{
	int	pid;

	while (data)
	{
		xpipe(data->pipefd);
		pid = xfork();
		if (pid == 0)
		{
			xclose(data->pipefd[READ]);
			if (data->next != NULL)
				ft_dup2(data->pipefd[WRITE], STDOUT_FILENO);
				// data->out_fd = data->pipefd[WRITE];
			if (setdata_cmdline_redirect(data) == 0)
				execute_command(data);
			exit(*(data->status));
		}
		else
		{
			xclose(data->pipefd[WRITE]);
			ft_dup2(data->pipefd[READ], STDIN_FILENO);
		}
		data = data->next;
	}
	return (pid);
}

static void	stdfd_handler(t_execdata *data, int mode)
{
	if (mode == SAVE)
	{
		data->ori_stdin = xdup(STDIN_FILENO);
		data->ori_stdout = xdup(STDOUT_FILENO);
		data->ori_stderr = xdup(STDERR_FILENO);
	}
	if (mode == RESTORE)
	{
		ft_dup2(data->ori_stdin, STDIN_FILENO);
		ft_dup2(data->ori_stdout, STDOUT_FILENO);
		ft_dup2(data->ori_stderr, STDERR_FILENO);
	}
}

void	execute_start(t_execdata *data)
{
	int			lastchild_pid;
	int			wstatus;

	setdata_heredoc_cmdtype(data);
	if (data->next == NULL && data->cmd_type != OTHER)
	{
		stdfd_handler(data, SAVE);
		if (setdata_cmdline_redirect(data) == 0)
			execute_command(data);
		free_2d_array(data->cmdline);
		stdfd_handler(data, RESTORE);
	}
	else
	{
		lastchild_pid = execute_loop(data);
		xwaitpid(lastchild_pid, &wstatus, 0);
		*(data->status) = WEXITSTATUS(wstatus);//confirm WIFEXITED and WIFSIGNALED etc..
		while(data->next)
		{
			xwaitpid(0, NULL, 0);
			data = data->next;
		}
	}
}