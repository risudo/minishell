#include "rakiyama.h"


int	setdata_cmdline_redirect(t_execdata *data)
{
	int			ret;
	t_iolist	*move;

	data->cmdline = convert_cmdlist_2dchar(data->clst);
	ret = 0;
	move = data->iolst;
	while(move)
	{
		if (move->c_type == IN_REDIRECT)
			ret = ft_dup2(ft_open(move->next->str, O_RDONLY, 0), STDIN_FILENO);
		else if (move->c_type == IN_HERE_DOC)
			ret = ft_dup2(move->here_doc_fd, STDIN_FILENO);
		else if (move->c_type == OUT_REDIRECT)
			ret = ft_dup2(ft_open(move->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0666), STDOUT_FILENO);
		else if (move->c_type ==  OUT_HERE_DOC)
			ret = ft_dup2(ft_open(move->next->str, O_WRONLY | O_CREAT | O_APPEND, 0666), STDOUT_FILENO);
		if (ret == -1)
		{
			*(data->status) = 1;
			break ;
		}
		move = move->next;
	}
	return (ret);
}

int	is_cmd_type(t_cmdlist *clst)
{
	if (clst == NULL)
		return (NON_CMD);
	else if (ft_strcmp(clst->str, "echo") == 0)
		return(ECHO);
	else if (ft_strcmp(clst->str, "cd") == 0)
		return(CD);
	else if (ft_strcmp(clst->str, "pwd") == 0)
		return(PWD);
	else if (ft_strcmp(clst->str, "export") == 0)
		return(EXPORT);
	else if (ft_strcmp(clst->str, "unset") == 0)
		return(UNSET);
	else if (ft_strcmp(clst->str, "env") == 0)
		return(ENV);
	else if (ft_strcmp(clst->str, "exit") == 0)
		return(EXIT);
	else
		return(OTHER);
}

enum e_cmd	get_here_doc(char *limiter)
{
	char*	line;
	int		pipefd[PIPEFD_NUM];

	xpipe(pipefd);
	while (1)
	{
		line = readline("> ");
		if (!line)
			exit(1);//tmp
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		else if (line[0] == '\0')
			ft_putstr_fd("\n", pipefd[WRITE]);
		else
			ft_putendl_fd(line, pipefd[WRITE]);
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
				move->here_doc_fd = get_here_doc(move->next->str);
			move = move->next;
		}
		data->cmd_type = is_cmd_type(data->clst);
		data = data->next;
	}
}