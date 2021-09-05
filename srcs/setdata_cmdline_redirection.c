#include "minishell.h"

/*
** Used to prepare data before executing command.
** 1 Make command line char array.
** 2 Redirection.
*/

static int	set_redirected_fd(t_execdata *data, t_iolist *iolst, \
							int *redirected_fd, int *is_fd_specified)
{
	if (iolst->c_type == FD)
	{
		*redirected_fd = ft_atoi(iolst->str);
		if (*redirected_fd < 0 || FD_MAX < *redirected_fd)
		{
			ft_putendl_fd("bad file descriptor", STDERR_FILENO);
			return (-1);
		}
		open_fd_handler(data, FD_REDIRECTED, *redirected_fd);
		*is_fd_specified = 1;
	}
	else if (*is_fd_specified == 0 && \
		(iolst->c_type == IN_REDIRECT || iolst->c_type == IN_HERE_DOC))
		*redirected_fd = STDIN_FILENO;
	else if (*is_fd_specified == 0 && \
		(iolst->c_type == OUT_REDIRECT || iolst->c_type == OUT_HERE_DOC))
		*redirected_fd = STDOUT_FILENO;
	return (0);
}

static int	redirection(t_execdata *data, t_iolist *iolst, \
					int redirected_fd, int *is_fd_specified)
{
	int	ret;

	ret = expand_filename(iolst->next, data->elst);
	if (ret == 0 && iolst->c_type == IN_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_RDONLY, 0), redirected_fd);
	else if (ret == 0 && iolst->c_type == IN_HERE_DOC)
		ret = ft_dup2(iolst->here_doc_fd, redirected_fd);
	else if (ret == 0 && iolst->c_type == OUT_REDIRECT)
		ret = ft_dup2(ft_open(iolst->next, O_WRONLY | O_CREAT | O_TRUNC, 0666), \
						redirected_fd);
	else if (ret == 0 && iolst->c_type == OUT_HERE_DOC)
		ret = ft_dup2(ft_open(iolst->next, O_WRONLY | O_CREAT | O_APPEND, 0666), \
						redirected_fd);
	*is_fd_specified = 0;
	return (ret);
}

int	setdata_cmdline_redirect(t_execdata *data)
{
	int			ret;
	t_iolist	*move;
	int			redirected_fd;
	int			is_fd_specified;

	data->cmdline = convert_cmdlist_2dchar(data->clst);
	ret = 0;
	is_fd_specified = 0;
	move = data->iolst;
	while (ret == 0 && move)
	{
		ret = set_redirected_fd(data, move, &redirected_fd, &is_fd_specified);
		if (ret == 0 && \
			IN_REDIRECT <= move->c_type && move->c_type <= OUT_HERE_DOC)
			ret = redirection(data, move, \
					redirected_fd, &is_fd_specified);
		move = move->next;
	}
	if (ret == -1)
		*(data->status) = 1;
	return (ret);
}