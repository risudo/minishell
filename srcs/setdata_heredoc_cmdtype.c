#include "minishell.h"

/*
** Used to prepare data before executing command.
** 1 Recieve all here documents and the pipe
**   (contains here documents) fd.
** 2 get command type of each data.
*/

static t_cmd	is_cmd_type(t_cmdlist *clst)
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

static void	expansion_heredoc(char **line, t_envlist *elst)
{
	char	*doll_ptr;
	size_t	len;

	doll_ptr = ft_strdoll(*line);
	while (doll_ptr)
	{
		len = expansion_key_io(line, elst, doll_ptr);
		doll_ptr = ft_strdoll(*line + len);
	}
}

static void	child_get_here_doc(char *limiter, t_execdata *data, \
									int is_quot, int pipewrite)
{
	char	*line;
	int		no_limit;

	no_limit = 1;
	while (no_limit && pipewrite != -1)
	{
		line = readline("> ");
		if (!line)
			break ;
		no_limit = ft_strcmp(line, limiter);
		if (no_limit)
		{
			if (is_quot == 0)
				expansion_heredoc(&line, data->elst);
			ft_putendl_fd(line, pipewrite);
		}
		free(line);
	}
	xclose(pipewrite);
	exit(0);
}

static int	get_here_doc(char *limiter, t_execdata *data, \
							int is_quot, t_iolist *iolst)
{
	int		pipefd[PIPEFD_NUM];
	int		wstatus;

	ft_pipe(pipefd);
	xsignal(SIGINT, SIG_IGN);
	if (xfork() == 0)
	{
		xsignal(SIGINT, SIG_DFL);
		xclose(pipefd[READ]);
		child_get_here_doc(limiter, data, is_quot, pipefd[WRITE]);
	}
	xclose(pipefd[WRITE]);
	iolst->open_fd = pipefd[READ];
	wait(&wstatus);
	xsignal(SIGINT, signal_handler);
	set_status_from_child_status(wstatus);
	if (g_status != 0)
		return (-1);
	return (0);
}

int	setdata_heredoc_cmdtype(t_execdata *data)
{
	t_iolist	*move;
	int			is_quot;

	while (data)
	{
		move = data->iolst;
		while (move)
		{
			is_quot = 0;
			if (move->c_type == IN_HERE_DOC)
			{
				if (ft_strchr(move->next->quot, '1') || \
					ft_strchr(move->next->quot, '2'))
					clear_quot_filename(&(move->next->str), \
						&(move->next->quot)), is_quot++;
				if (get_here_doc(move->next->str, data, \
									is_quot, move) == -1)
					return (-1);
			}
			move = move->next;
		}
		data->cmd_type = is_cmd_type(data->clst);
		data = data->next;
	}
	return (0);
}
