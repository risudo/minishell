#include "rakiyama.h"

/*
set
data->cmdline
data->in_fd
data->out_fd
*/
// int	set_execdata(t_execdata *data)
// {
// 	t_iolist	*move;

// 	data->cmdline = convert_cmdlist_2dchar(data->clst);
// 	move = data->iolst;
// 	while(move)
// 	{
// 		if (move->c_type == IN_REDIRECT)
// 			data->in_fd = ft_open(data->in_fd, move->next->str, O_RDONLY, 0);
// 		else if (move->c_type == IN_HERE_DOC)
// 			data->in_fd = move->here_doc_fd;
// 		else if (move->c_type == OUT_REDIRECT)
// 			data->out_fd = ft_open(data->out_fd, move->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 		else if (move->c_type ==  OUT_HERE_DOC)
// 			data->out_fd = ft_open(data->out_fd, move->next->str, O_WRONLY | O_CREAT | O_APPEND, 0666);
// 		if (data->in_fd < 0 || data->out_fd < 0)
// 		{
// 			*(data->status) = 1;
// 			return (-1);
// 		}
// 		move = move->next;
// 	}
// 	xdup2(data->in_fd, STDIN_FILENO);
// 	xdup2(data->out_fd, STDOUT_FILENO);
// 	return (0);
// }


int	set_execdata(t_execdata *data)
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

void	execute_command(t_execdata *data)
{
	void	(*cmd_func[CMD_NUM])(t_execdata *data);

	cmd_func[ECHO] = builtin_echo;
	cmd_func[CD] = builtin_cd;
	cmd_func[PWD] = builtin_pwd;
	cmd_func[EXPORT] = builtin_export;
	cmd_func[UNSET] = builtin_unset;
	cmd_func[ENV] = builtin_env;
	cmd_func[EXIT] = builtin_exit;
	cmd_func[OTHER] = non_builtin;
	cmd_func[NON_CMD] = non_command;
	cmd_func[data->cmd_type](data);
}

/*
if pipe exists in input,
this loop execute each command and connect those input and output
*/
int	execute_loop(t_execdata *data)
{
	int	pid;

	while (data)
	{
		xpipe(data->pipefd);
		pid = xfork();
		if (pid == 0)
		{
			xclose(data->pipefd[READ]);
			if (data->next != NULL)
				ft_dup2(data->pipefd[WRITE], STDOUT_FILENO);
				// data->out_fd = data->pipefd[WRITE];
			if (set_execdata(data) == 0)
				execute_command(data);
			exit(*(data->status));
		}
		else
		{
			xclose(data->pipefd[WRITE]);
			ft_dup2(data->pipefd[READ], STDIN_FILENO);
		}
		data = data->next;
	}
	return (pid);
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

void	set_heredocfd_cmdtype(t_execdata *data)
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

// void	execute_start(t_execdata *data)
// {
// 	int			lastchild_pid;
// 	int			wstatus;

// 	set_heredocfd_cmdtype(data);
// 	if (data->next == NULL && 
// 		(data->cmd_type == CD || (data->cmd_type == EXPORT && data->clst->next) || data->cmd_type == UNSET || data->cmd_type == EXIT))
// 	{
// 		if (set_execdata(data) == 0)
// 			execute_command(data);
// 		free_2d_array(data->cmdline);
// 	}
// 	else
// 	{
// 		lastchild_pid = execute_loop(data);
// 		xwaitpid(lastchild_pid, &wstatus, 0);
// 		*(data->status) = WEXITSTATUS(wstatus);//confirm WIFEXITED and WIFSIGNALED etc..
// 		while(data->next)
// 		{
// 			xwaitpid(0, NULL, 0);
// 			data = data->next;
// 		}
// 	}
// }

void	execute_start(t_execdata *data)
{
	int			ori_stdin;
	int			ori_stdout;
	int			lastchild_pid;
	int			wstatus;

	set_heredocfd_cmdtype(data);
	if (data->next == NULL && data->cmd_type != OTHER)
	{
		ori_stdin = xdup(STDIN_FILENO);
		ori_stdout = xdup(STDOUT_FILENO);
		if (set_execdata(data) == 0)
			execute_command(data);
		free_2d_array(data->cmdline);
		ft_dup2(ori_stdin, STDIN_FILENO);
		ft_dup2(ori_stdout, STDOUT_FILENO);
	}
	else
	{
		lastchild_pid = execute_loop(data);
		xwaitpid(lastchild_pid, &wstatus, 0);
		*(data->status) = WEXITSTATUS(wstatus);//confirm WIFEXITED and WIFSIGNALED etc..
		while(data->next)
		{
			xwaitpid(0, NULL, 0);
			data = data->next;
		}
	}
}