#include "minishell.h"

/*
**	Used to prepare data before executing command.
*/

int	setdata_cmdline_redirect(t_execdata *data)
{
	int			ret;
	t_iolist	*move;

	data->cmdline = convert_cmdlist_2dchar(data->clst);
	ret = 0;
	move = data->iolst;
	while (ret == 0 && move)
	{
		if (move->c_type == IN_REDIRECT)
			ret = ft_dup2(ft_open(move->next, O_RDONLY, 0), STDIN_FILENO);
		else if (move->c_type == IN_HERE_DOC)
			ret = ft_dup2(move->here_doc_fd, STDIN_FILENO);
		else if (move->c_type == OUT_REDIRECT)
			ret = ft_dup2(ft_open(move->next, O_WRONLY | O_CREAT | O_TRUNC, 0666), \
							STDOUT_FILENO);
		else if (move->c_type == OUT_HERE_DOC)
			ret = ft_dup2(ft_open(move->next, O_WRONLY | O_CREAT | O_APPEND, 0666), \
							STDOUT_FILENO);
		if (ret == -1)
			*(data->status) = 1;
		move = move->next;
	}
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

static t_cmd	get_here_doc(char *limiter, t_envlist *elst)
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
			expansion_key_heredoc(&line, elst, ft_strchr(line, '$'));
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

	while (data)
	{
		move = data->iolst;
		while (move)
		{
			if (move->c_type == IN_HERE_DOC)
				move->here_doc_fd = get_here_doc(move->next->str, data->elst);
			move = move->next;
		}
		data->cmd_type = is_cmd_type(data->clst);
		data = data->next;
	}
}
