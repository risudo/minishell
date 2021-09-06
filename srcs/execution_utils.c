#include "minishell.h"

t_path_type	ft_stat(char *pathname)
{
	struct stat	sb;

	if (stat(pathname, &sb) == -1)
		return (UNKNOWN);
	if ((sb.st_mode & S_IFMT) == S_IFREG)
		return (IS_FILE);
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (IS_DIR);
	return (ELSE_TYPE);
}

int	ft_dup2(int oldfd, int newfd, int exit_status)
{
	int	fd;

	fd = 0;
	if (oldfd < 0 || newfd < 0)
		return (-1);
	if (oldfd != newfd)
	{
		fd = dup2(oldfd, newfd);
		if (fd == -1)
		{
			ft_putstr_fd("dup2 : ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			if (exit_status)
				exit(exit_status);
		}
		xclose(oldfd);
	}
	return (fd);
}

int	ft_open(t_iolist *filenode, int flags, mode_t mode)
{
	int	fd;

	if (mode == 0)
		fd = open(filenode->str, flags);
	else
		fd = open(filenode->str, flags, mode);
	if (fd == -1)
	{
		ft_putstr_fd("open: ", STDERR_FILENO);
		ft_putstr_fd(filenode->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
	return (fd);
}

void	execute_command(t_execdata *data)
{
	void	(*cmd_func[CMD_NUM])(t_execdata *data);

	if (data->cmd_type == NON_CMD)
		return ;
	cmd_func[ECHO] = builtin_echo;
	cmd_func[CD] = builtin_cd;
	cmd_func[PWD] = builtin_pwd;
	cmd_func[EXPORT] = builtin_export;
	cmd_func[UNSET] = builtin_unset;
	cmd_func[ENV] = builtin_env;
	cmd_func[EXIT] = builtin_exit;
	cmd_func[OTHER] = non_builtin;
	cmd_func[NON_CMD] = NULL;
	cmd_func[data->cmd_type](data);
}

void	open_fd_handler(t_execdata *data, int mode, int redireceted_fd)
{
	static char	open_fd_flag[FD_MAX + 1];
	int			index_fd;

	if (mode == FD_REDIRECTED && !open_fd_flag[redireceted_fd])
	{
		if (data->ori_stdin == redireceted_fd)
			data->ori_stdin = xdup(redireceted_fd);
		else if (data->ori_stdout == redireceted_fd)
			data->ori_stdout = xdup(redireceted_fd);
		else if (data->ori_stderr == redireceted_fd)
			data->ori_stderr = xdup(redireceted_fd);
		open_fd_flag[redireceted_fd]++;
	}
	else if (mode == ALL_CLOSE)
	{
		index_fd = -1;
		while (++index_fd <= FD_MAX)
		{
			if (open_fd_flag[index_fd])
				xclose(index_fd), open_fd_flag[index_fd] = 0;
		}
	}
}
