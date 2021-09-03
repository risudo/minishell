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

int	ft_dup2(int oldfd, int newfd)
{
	if (oldfd < 0 || newfd < 0)
		return (-1);
	if (oldfd != newfd)
	{
		xdup2(oldfd, newfd);
		xclose(oldfd);
	}
	return (0);
}

int	ft_open(t_iolist *filenode, int flags, mode_t mode)
{
	int	fd;

	if (filenode->next && filenode->next->c_type == ELSE)
	{
		ft_putendl_fd("minishell: ambiguous redirect", STDERR_FILENO);
		return (-1);
	}
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
