# include "rakiyama.h"

/*check if the file exists*/
int	ft_stat(char *pathname)
{
	struct stat sb;

	if (stat(pathname, &sb) == -1)
		return (0);
	else
		return (1);
/*	{
		ft_putstr_fd("stat : ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (S_ISREG(sb.st_mode))
		return (1);
	else
		return (0);
*/
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

// int	ft_open(int	old_fd, char *filepath, int flags, mode_t mode)
// {
// 	int	fd;

// 	if (STDERR_FILENO < old_fd)
// 		xclose(old_fd);
// 	if (mode == 0)
// 		fd = open(filepath, flags);
// 	else
// 		fd = open(filepath, flags, mode);
// 	if (fd == -1)
// 	{
// 		ft_putstr_fd("open: ", STDERR_FILENO);
// 		ft_putstr_fd(filepath, STDERR_FILENO);
// 		ft_putstr_fd(": ", STDERR_FILENO);
// 		perror("");
// 	}
// 	return (fd);
// }

int	ft_open(char *filepath, int flags, mode_t mode)
{
	int	fd;

	if (mode == 0)
		fd = open(filepath, flags);
	else
		fd = open(filepath, flags, mode);
	if (fd == -1)
	{
		ft_putstr_fd("open: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
	return (fd);
}