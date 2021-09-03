#include "minishell.h"

/*
** Used to prepare data before executing command.
*/

static int	set_redirected_fd(t_execdata *data, t_iolist *iolst, int *redirect_fd, int *is_fd_specified)
{
/*	if (iolst->c_type == FD)
	{
		*redirect_fd = ft_atoi(iolst->str);
		if (*redirect_fd < 0 || FD_MAX < *redirect_fd)
		{
			ft_putendl_fd("bad file descriptor", STDERR_FILENO);
			return (-1);
		}
		stdfd_handler(data, FD_REDIRECTED, *redirected_fd);
		*is_fd_specified = 1;
	}*/
	(void)data;
	if (*is_fd_specified == 0 && \
		(iolst->c_type == IN_REDIRECT || iolst->c_type == IN_HERE_DOC))
		*redirect_fd = STDIN_FILENO;
	else if (*is_fd_specified == 0 && \
		(iolst->c_type == OUT_REDIRECT || iolst->c_type == OUT_HERE_DOC))
	*redirect_fd = STDOUT_FILENO;
	return (0);
}

static int	redirection(t_iolist *iolst, t_envlist *elst, int redirected_fd, int *is_fd_specified)
{
	int	ret;

	ret = serch_env_iolist(iolst->next, elst);
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
		if (ret == 0 && IN_REDIRECT <= move->c_type && move->c_type <= OUT_HERE_DOC)
			ret = redirection(move, data->elst, redirected_fd, &is_fd_specified);
		move = move->next;
	}
	if (ret == -1)
		*(data->status) = 1;
	return (ret);
}

static int	is_cmd_type(t_cmdlist *clst)
{
	if (clst == NULL)
		return (NON_CMD);
	else if (ft_strcmp(clst->str, "echo") == 0)
		return (ECHO);
	else if (ft_strcmp(clst->str, "cd") == 0)
		return (CD);
	else if (ft_strcmp(clst->str, "pwd") == 0)
		return (PWD);
	else if (ft_strcmp(clst->str, "export") == 0)
		return (EXPORT);
	else if (ft_strcmp(clst->str, "unset") == 0)
		return (UNSET);
	else if (ft_strcmp(clst->str, "env") == 0)
		return (ENV);
	else if (ft_strcmp(clst->str, "exit") == 0)
		return (EXIT);
	else
		return (OTHER);
}

static t_cmd	get_here_doc(char *limiter, t_envlist *elst, int is_quot)
{
	char	*line;
	int		pipefd[PIPEFD_NUM];
	int		no_limit;

	xpipe(pipefd);
	no_limit = 1;
	while (no_limit)
	{
		line = readline("> ");
		if (!line)
			exit(1);
		no_limit = ft_strcmp(line, limiter);
		if (no_limit)
		{
			if (is_quot == 0)
				expansion_key_heredoc(&line, elst, ft_strchr(line, '$'), 1);
			ft_putendl_fd(line, pipefd[WRITE]);
		}
		free(line);
	}
	xclose(pipefd[WRITE]);
	return (pipefd[READ]);
}

void	setdata_heredoc_cmdtype(t_execdata *data)
{
	t_iolist	*move;
	int			is_quot;

	is_quot = 0;
	while (data)
	{
		move = data->iolst;
		while (move)
		{
			if (move->c_type == IN_HERE_DOC)
			{
				if (ft_strchr(move->next->quot, '1') || ft_strchr(move->next->quot, '2'))
				{
					clear_quot_iolist(move->next);
					is_quot++;
				}
				move->here_doc_fd = get_here_doc(move->next->str, data->elst, is_quot);
			}
			move = move->next;
		}
		data->cmd_type = is_cmd_type(data->clst);
		data = data->next;
	}
}
