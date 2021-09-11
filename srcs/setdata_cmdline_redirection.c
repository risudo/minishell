#include "minishell.h"

/*
** Used to prepare data before executing command.
** 1 Make command line char array.
** 2 Redirection.
*/

static int	resave_stdfd(t_execdata *data, int fd)
{
	if (data->stdfd[ORIGINAL_IN] == fd)
		return (ft_dup(data, ORIGINAL_IN, fd));
	else if (data->stdfd[ORIGINAL_OUT] == fd)
		return (ft_dup(data, ORIGINAL_OUT, fd));
	else if (data->stdfd[ORIGINAL_ERR] == fd)
		return (ft_dup(data, ORIGINAL_ERR, fd));
	return (0);
}

static int	set_redirect_fd(t_execdata *data, t_iolist *iolst, \
							int *redirect_fd, int *is_fd_specified)
{
	if (iolst->c_type == FD)
	{
		*redirect_fd = ft_atoi(iolst->str);
		if (resave_stdfd(data, *redirect_fd) == -1)
			return (-1);
		*is_fd_specified = 1;
	}
	else if (*is_fd_specified == 0 && \
		(iolst->c_type == IN_REDIRECT || iolst->c_type == IN_HERE_DOC))
		*redirect_fd = STDIN_FILENO;
	else if (*is_fd_specified == 0 && \
		(iolst->c_type == OUT_REDIRECT || iolst->c_type == OUT_HERE_DOC))
		*redirect_fd = STDOUT_FILENO;
	return (0);
}

static int	redirection(t_execdata *data, t_iolist *iolst, \
					int redirect_fd, int *is_fd_specified)
{
	int	ret;

	ret = 0;
	if (iolst->c_type != IN_HERE_DOC)
		ret = expand_filename(iolst->next, data->elst);
	if (ret != -1 && iolst->c_type == IN_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_RDONLY, 0), redirect_fd);
	else if (ret != -1 && iolst->c_type == IN_HERE_DOC)
	{
		ret = ft_dup2(iolst->open_fd, redirect_fd);
		iolst->open_fd = -1;
	}
	else if (ret != -1 && iolst->c_type == OUT_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_WRONLY | O_CREAT | O_TRUNC, 0666), \
						redirect_fd);
	else if (ret != -1 && iolst->c_type == OUT_HERE_DOC)
		ret = ft_dup2(\
				ft_open(iolst->next, O_WRONLY | O_CREAT | O_APPEND, 0666), \
						redirect_fd);
	if (ret != -1 && STDERR_FILENO < redirect_fd)
		iolst->open_fd = redirect_fd;
	*is_fd_specified = 0;
	return (ret);
}

int	setdata_cmdline_redirect(t_execdata *data)
{
	int			ret;
	t_iolist	*move;
	int			redirect_fd;
	int			is_fd_specified;

	data->cmdline = convert_cmdlist_2dchar(data->clst);
	ret = 0;
	is_fd_specified = 0;
	move = data->iolst;
	while (ret != -1 && move)
	{
		ret = set_redirect_fd(data, move, &redirect_fd, &is_fd_specified);
		if (ret != -1 && \
			IN_REDIRECT <= move->c_type && move->c_type <= OUT_HERE_DOC)
			ret = redirection(data, move, \
					redirect_fd, &is_fd_specified);
		if (ret == -1)
			g_status = 1;
		else
			g_status = 0;
		move = move->next;
	}
	return (ret);
}
