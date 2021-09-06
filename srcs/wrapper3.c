#include "minishell.h"

int	ft_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		pipefd[READ] = -1;
		pipefd[WRITE] = -1;
		return (-1);
	}
	return (0);
}

int	ft_dup(t_execdata *data, t_stdfd type, int oldfd)
{
	int	newfd;

	newfd = dup(oldfd);
	if (newfd == -1)
	{
		ft_putstr_fd("dup : ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
	else
		data->stdfd[type] = newfd;
	return (newfd);
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
