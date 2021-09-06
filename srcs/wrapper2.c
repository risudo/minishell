#include "minishell.h"

char	*ft_xstrjoin(char *str1, char *str2)
{
	char	*joined_str;

	joined_str = ft_strjoin(str1, str2);
	if (!joined_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (joined_str);
}

char	**ft_xsplit(char *src_str, char cut_char)
{
	char	**splited_str;

	splited_str = ft_split(src_str, cut_char);
	if (!splited_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (splited_str);
}

void	xclose(int fd)
{
	if (close(fd) == -1)
	{
		ft_putstr_fd("close : ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
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
