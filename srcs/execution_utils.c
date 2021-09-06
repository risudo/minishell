#include "minishell.h"

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

int	redirect_fd_handler(t_execdata *data, t_fd_mode mode, int fd)
{
	static char	redfd_flag[FD_MAX + 1];
	int			index_fd;

	if (mode == FD_SPECIFIED)
	{
		if (data->stdfd[ORIGINAL_IN] == fd)
			return (ft_dup(data, ORIGINAL_IN, fd));
		else if (data->stdfd[ORIGINAL_OUT] == fd)
			return (ft_dup(data, ORIGINAL_OUT, fd));
		else if (data->stdfd[ORIGINAL_ERR] == fd)
			return (ft_dup(data, ORIGINAL_ERR, fd));
	}
	else if (mode == FD_REDIRECTED && \
				!redfd_flag[fd] && STDERR_FILENO < fd)
		redfd_flag[fd]++;
	else if (mode == ALL_CLOSE)
	{
		index_fd = -1;
		while (++index_fd <= FD_MAX)
		{
			if (redfd_flag[index_fd])
				xclose(index_fd), redfd_flag[index_fd] = 0;
		}
	}
	return (0);
}
