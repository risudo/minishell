#include "minishell.h"

/*
** Used to prepare data before executing command.
** 1 Make command line char array.
** 2 Redirection.
*/

static int	set_redirect_fd(t_execdata *data, t_iolist *iolst, \
							int *redirect_fd, int *is_fd_specified)
{
	if (iolst->c_type == FD)
	{
		*redirect_fd = ft_atoi(iolst->str);
		if (*redirect_fd < 0 || FD_MAX < *redirect_fd)
		{
			ft_putendl_fd("bad file descriptor", STDERR_FILENO);
			return (-1);
		}
		if (redfd_handler(data, FD_SPECIFIED, *redirect_fd) == -1)
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

	ret = expand_filename(iolst->next, data->elst);
	if (ret != -1 && iolst->c_type == IN_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_RDONLY, 0), redirect_fd, 0);
	else if (ret != -1 && iolst->c_type == IN_HERE_DOC)
		ret = ft_dup2(iolst->here_doc_fd, redirect_fd, 0);
	else if (ret != -1 && iolst->c_type == OUT_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_WRONLY | O_CREAT | O_TRUNC, 0666), \
						redirect_fd, 0);
	else if (ret != -1 && iolst->c_type == OUT_HERE_DOC)
		ret = ft_dup2(ft_open(iolst->next, O_WRONLY | O_CREAT | O_APPEND, 0666), \
						redirect_fd, 0);
	if (ret != -1)
		redfd_handler(data, FD_REDIRECTED, redirect_fd);
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
		move = move->next;
	}
	if (ret == -1)
		g_status = 1;
	return (ret);
}
