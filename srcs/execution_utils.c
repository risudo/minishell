#include "minishell.h"

/*check if the file exists*/
int	ft_stat(char *pathname)
{
	struct stat	sb;

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

void	expansion_key_heredoc(char **line,
		t_envlist *envlist, char *doll_ptr)
{
	char	*key;
	char	*value;
	char	*front_key;
	char	*back_key;
	size_t	len;

	while (doll_ptr)
	{
		len = 1;
		while (!is_delimiter(doll_ptr[len]))
			len++;
		front_key = ft_xsubstr(*line, 0, doll_ptr - *line);
		key = ft_xsubstr(doll_ptr, 1, len - 1);
		back_key = ft_xsubstr(doll_ptr + len, 0, ft_strlen(doll_ptr + len));
		value = ft_getenv(envlist, key);
		xfree(*line);
		*line = ft_strjoin_three(front_key, value, back_key);
		xfree(front_key), xfree(key), xfree(back_key);
		doll_ptr = ft_strchr(*line, '$');
	}
}
