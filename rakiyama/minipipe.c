#include "rakiyama.h"

/*
enum e_special_c
{
	IN_REDIRECT,
	OUT_REDIRECT,
	IN_HERE_DOC,
	OUT_HERE_DOC,
	PIPE,
	S_TYPE_NUM
};

enum e_cmd
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	OTHER,
	NON_CMD,
	CMD_NUM
};

enum e_pipefd
{
	READ,
	WRITE,
	PIPEFD_NUM
};

typedef struct s_token
{
	char				*str;
	struct s_token		*next;
	struct s_token		*prev;
}   t_token;

typedef struct s_cmdlist
{
	char				*str;
	struct s_cmdlist	*next;
	struct s_cmdlist	*prev;
}   t_cmdlist;

typedef struct s_iolist
{
	enum e_special_c	c_type;
	char				*str;
	int					here_doc_fd;
	struct s_iolist		*next;
	struct s_iolist		*prev;
}   t_iolist;

typedef struct s_envlist
{
	char				*key;
	char				*value;
	struct s_envlist	*next;
	struct s_envlist	*prev;
}   t_envlist;

typedef struct s_execdata
{
	char			**cmdline;
	int				in_fd;
	int				out_fd;
	int				*status;
	int				pipefd[PIPEFD_NUM];
	enum e_cmd		cmd_type;
	t_cmdlist		*clst;
	t_iolist		*iolst;
	t_envlist		*elst;
	struct s_execdata	*next;
}   t_execdata;
*/

void	free_2d_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
	}
}

void	put_2d_array(char **a)
{
	if (a == NULL)
	{
		fprintf(stderr, "array is NULL");
		return ;
	}
	while (*a != NULL)
	{
		fprintf(stderr, "%s\n", *a);
		a++;
	}
}

int	envlist_size(t_envlist *elst)
{
	t_envlist	*move;
	int			cnt;

	cnt = 0;
	move = elst;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

/*check if the file exists*/
int	ft_stat(char *pathname)
{
	struct stat sb;

	if (stat(pathname, &sb) == -1)
		return (0);
	else
		return (1);
/*	{
		ft_putstr_fd("stat : ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (S_ISREG(sb.st_mode))
		return (1);
	else
		return (0);
*/
}

/*
split PATH and join "/" + "command"
example (command is cat)
/usr/local/bin/cat
/usr/bin/cat
/bin/cat
:
finally return the list
*/
char	**make_exec_pathlist(char *cmd, char *path_env)
{
	char	**pathlist;
	char	*tmp;
	int		i;

	pathlist = ft_xsplit(path_env, ':'); 
	i = 0;
	while (pathlist[i])
	{
		tmp = pathlist[i];
		pathlist[i] = ft_xstrjoin(pathlist[i], "/");
		free(tmp);
		tmp = pathlist[i];
		pathlist[i] = ft_xstrjoin(pathlist[i], cmd);
		free(tmp);
		i++;
	}
	return (pathlist);
}

/*
search the valid executable file path in pathlist.
*/
char	*set_cmd_path(char *cmd, char *path_env)
{
	char		**pathlist;
	char		*cmd_path;
	int			i;

	if (ft_stat(cmd))
		return (cmd);
	if (path_env == NULL)
		pathlist = NULL;
	else
		pathlist = make_exec_pathlist(cmd, path_env);
	cmd_path = NULL;
	i = 0;
	while(pathlist && pathlist[i])
	{
		if (ft_stat(pathlist[i]))
		{
			cmd_path = ft_xstrjoin(pathlist[i], NULL);
			break ;
		}
		i++;
	}
	free_2d_array(pathlist);
	return (cmd_path);
}

char	*ft_getenv(t_envlist *elst, char *search)
{
	while (elst)
	{
		if (ft_strncmp(elst->key, search, ft_strlen(search) + 1) == 0)
			return (elst->value);
		elst = elst->next;
	}
	return (NULL);
}

t_envlist	*ft_unsetenv(t_envlist *elst, char *rm_key)
{
	t_envlist	*head;
	t_envlist	*prev;

	head = elst;
	prev = NULL;
	while (elst)
	{
		if (ft_strncmp(elst->key, rm_key, ft_strlen(rm_key) + 1) == 0)
		{
			if (prev == NULL)
				head = elst->next;
			else
				prev->next = elst->next;
//			free(elst->key);
//			free(elst->value);
			free(elst);
			break ;
		}
		prev = elst;
		elst = elst->next;
	}
	return (head);
}

void	ft_setenv(t_envlist *elst, char *add)
{
	t_envlist	*new;

	new = ft_xcalloc(1, sizeof(*new));
	set_key_and_value(add, new);
	while(elst)
	{
		if (ft_strncmp(elst->key, new->key, ft_strlen(new->key) + 1) == 0)
		{
			//free(elst->value)
			elst->value = new->value;
			free(new->key);
			free(new);
			return ;
		}
		if (elst->next == NULL)
			break ;
		elst = elst->next;
	}
	elst->next = new;
}

/*
convert t_envlist type list -> char type 2d array
*/
char	**convert_envlist_2dchar(t_envlist *elst)
{
	char 		**array;
	t_envlist	*move;
	int			cnt;
	char		*tmp;

	cnt = envlist_size(elst);
	array = (char **)ft_xcalloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = elst;
	while (move)
	{
		array[cnt] = ft_xstrjoin(move->key, "=");
		tmp = array[cnt];
		array[cnt] = ft_xstrjoin(array[cnt], move->value);
		free(tmp);
		cnt++;
		move = move->next;
	}
	return (array);
}

void	ft_execve(t_execdata *data)
{
	char	*cmd_path;

	*(data->status) = 0;
	cmd_path = set_cmd_path(data->cmdline[0], ft_getenv(data->elst, "PATH"));
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(data->cmdline[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (execve(cmd_path, data->cmdline, convert_envlist_2dchar(data->elst)) == -1)
		perror(data->cmdline[0]);
	*(data->status) = 126;
}

void	ft_echo(t_execdata *data)
{
	int	option;
	int	arg_i;

	option = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] && ft_strncmp(data->cmdline[arg_i], "-n", 5) == 0)
	{
		option++;
		arg_i++;
	}
	while (data->cmdline[arg_i])
	{
		printf("%s", data->cmdline[arg_i]);
		arg_i++;
	}
	if (option == 0)
		printf("\n");
	*(data->status) = 0;
}

void	ft_cd(t_execdata *data)
{
	char	*path;

	if (data->cmdline[1] == NULL)
	{
		path = ft_getenv(data->elst, "HOME");
		if (path == NULL)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			*(data->status) = 1;
			return ;
		}
	}
	else
		path = data->cmdline[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		*(data->status) = 1;
		return ;
	}
	*(data->status) = 0;//change env? old pwd pwd
}

void	ft_pwd(t_execdata *data)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		perror("pwd");
		*(data->status) = 1;
	}
	else
	{
		printf("%s\n", pathname);
		*(data->status) = 0;
	}
	free(pathname);
}

int	check_envname_rules(char *str)
{
	int	cnt_equal;
	int	i;

	cnt_equal = 0;
	i = 0;
	while(str[i])
	{
		if (str[i])
			/*rules*/;
		if (i == 0 && ('0' <=  str[i] || str[i] <= '9'))
			return (1);
		if (str[i] == '=')
			cnt_equal++;
	}
	if (cnt_equal == 0)
		return (1);
	return (0);
}

void	ft_export(t_execdata *data)
{
	int	arg_i;

	arg_i = 1;
	if (data->cmdline[arg_i] == NULL)
		printf("no argument\n");//
	while (data->cmdline[arg_i])
	{
		if (check_envname_rules(data->cmdline[arg_i]) == 0)
			ft_setenv(data->elst, data->cmdline[arg_i]);
		else
		{
			printf("error\n");//tmp
		}
		arg_i++;
	}
	*(data->status) = 0;
}

void	ft_unset(t_execdata *data)
{
	int i;

	i = 1;
	while (data->cmdline[i])
	{
		ft_unsetenv(data->elst, data->cmdline[1]);
		i++;
	}
	*(data->status) = 0;
}

void	ft_env(t_execdata *data)
{
	t_envlist	*move;

	move = data->elst;
	while(move)
	{
		printf("%s=%s\n", move->key, move->value);
		move = move->next;
	}
	*(data->status) = 0;
}

void	ft_exit(t_execdata *data)
{
	exit(0);
}

void	ft_non_cmd(t_execdata *data)
{
	*(data->status) = 0;
}

int	ft_open(int	old_fd, char *filepath, int flags, mode_t mode)
{
	int	fd;

	if (STDERR_FILENO < old_fd)
		xclose(old_fd);
	if (mode < 0)
		fd = open(filepath, flags);
	else
		fd = open(filepath, flags, mode);
	if (fd == -1)
	{
		ft_putstr_fd("open: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
	return (fd);
}

/*
convert t_cmdlist type list -> char type 2d array
*/
char	**convert_cmdlist_2dchar(t_cmdlist *clst)
{
	char 		**array;
	t_cmdlist	*move;
	int			cnt;

	cnt = 0;
	move = clst;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	array = (char **)ft_xcalloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = clst;
	while (move)
	{
		array[cnt] = ft_xstrjoin(move->str, NULL);
		cnt++;
		move = move->next;
	}
	return (array);
}

/*
set
data->cmdline
data->in_fd
data->out_fd
*/
int	set_execdata(t_execdata *data)
{
	t_iolist	*move;

	data->cmdline = convert_cmdlist_2dchar(data->clst);
	move = data->iolst;
	while(move)
	{
		if (move->c_type == IN_REDIRECT)
			data->in_fd = ft_open(data->in_fd, move->next->str, O_RDONLY, -1);//option ok?
		else if (move->c_type == IN_HERE_DOC)
			data->in_fd = move->here_doc_fd;
		else if (move->c_type == OUT_REDIRECT)
			data->out_fd = ft_open(data->out_fd, move->next->str, O_RDWR | O_TRUNC | O_CREAT, S_IREAD | S_IWRITE);
		else if (move->c_type ==  OUT_HERE_DOC)
			data->out_fd = ft_open(data->out_fd, move->next->str, O_RDWR | O_APPEND | O_CREAT, S_IREAD | S_IWRITE);
		if (data->in_fd < 0 || data->out_fd < 0)
		{
			*(data->status) = 1;
			return (-1);
		}
		move = move->next;
	}
	return (0);
}

int	dup_io(t_execdata *data)
{
	if (data->in_fd != STDIN_FILENO)
	{
		xdup2(data->in_fd, STDIN_FILENO);
		xclose(data->in_fd);
	}
	if (data->out_fd != STDOUT_FILENO)
	{
		xdup2(data->out_fd, STDOUT_FILENO);
		xclose(data->out_fd);
	}
	return (0);
}

void	to_cmd(t_execdata *data)
{
	void	(*cmd_func[CMD_NUM])(t_execdata *data);

	cmd_func[ECHO] = ft_echo;
	cmd_func[CD] = ft_cd;
	cmd_func[PWD] = ft_pwd;
	cmd_func[EXPORT] = ft_export;
	cmd_func[UNSET] = ft_unset;
	cmd_func[ENV] = ft_env;
	cmd_func[EXIT] = ft_exit;
	cmd_func[OTHER] = ft_execve;
	cmd_func[NON_CMD] = ft_non_cmd;
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
				data->out_fd = data->pipefd[WRITE];
			if (set_execdata(data) == 0 && dup_io(data) == 0)
				to_cmd(data);
			exit(*(data->status));
		}
		else
		{
			xclose(data->pipefd[WRITE]);
			xdup2(data->pipefd[READ], STDIN_FILENO);
			xclose(data->pipefd[READ]);
		}
		data = data->next;
	}
	return (pid);
}

t_cmdlist	*add_cmdlist(t_cmdlist *clst, char *s)
{
	t_cmdlist	*tmp;
	t_cmdlist	*move;

	tmp = (t_cmdlist *)malloc(sizeof(t_cmdlist));
	tmp->str = s;
	tmp->next = NULL;
	if (clst == NULL)
		return(tmp);
	move = clst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (clst);
}

t_iolist	*add_iolist(t_iolist *iolst, int c, char *s, int h)
{
	t_iolist	*tmp;
	t_iolist	*move;

	tmp = (t_iolist *)malloc(sizeof(t_iolist));
	tmp->c_type = c;
	tmp->str = s;
	tmp->here_doc_fd = h;
	tmp->next = NULL;
	if (iolst == NULL)
		return(tmp);
	move = iolst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (iolst);
}

t_envlist	*add_envlist(t_envlist *elst, char *k, char *v)
{
	t_envlist	*tmp;
	t_envlist	*move;

	tmp = (t_envlist *)malloc(sizeof(t_envlist));
	tmp->key = k;
	tmp->value = v;
	tmp->next = NULL;
	if (elst == NULL)
		return(tmp);
	move = elst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (elst);
}

t_execdata	*add_execdata(t_execdata *data, int *s, int c, t_cmdlist *clst, t_iolist *iolst, t_envlist *elst)
{
	t_execdata	*tmp;
	t_execdata	*move;

	tmp = (t_execdata *)malloc(sizeof(t_execdata));
	tmp->cmdline = NULL;
	tmp->in_fd = STDIN_FILENO;
	tmp->out_fd = STDOUT_FILENO;
	tmp->status = s;
//	tmp->cmd_type = c;
	tmp->clst = clst;
	tmp->iolst = iolst;
	tmp->elst = elst;
	tmp->next = NULL;
	if (data == NULL)
		return(tmp);
	move = data;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (data);
}

void	put_data(t_execdata *data)
{
	t_cmdlist	*ctmp;
	t_iolist	*iotmp;
	t_envlist	*etmp;

	while(data)
	{
		printf("--------------------------------------------\n");
		printf("status:%d\n", *(data->status));
		ctmp = data->clst;
		while (ctmp)
		{
			printf("cmdlist:%s\n", ctmp->str);
			ctmp = ctmp->next;
		}
		iotmp = data->iolst;
		while (iotmp)
		{
			printf("iolist:%s\n", iotmp->str);
			iotmp = iotmp->next;
		}
		etmp = data->elst;
		while (etmp)
		{
			printf("env key:%s\n", etmp->key);
			printf("env value:%s\n", etmp->value);
			etmp = etmp->next;
		}
		data = data->next;
	}
}

void	free_data(t_execdata *data)
{
	void		*tmp;

	free(data->status);
	while (data->elst)
	{
		tmp = data->elst;
		data->elst = data->elst->next;
		free(tmp);
	}
	while(data)
	{
		while (data->clst)
		{
			tmp = data->clst;
			data->clst = data->clst->next;
			free(tmp);
		}
		while (data->iolst)
		{
			tmp = data->iolst;
			data->iolst = data->iolst->next;
			free(tmp);
		}
		tmp = data;
		data = data->next;
		free(tmp);
	}
}

void	execute_start(t_execdata *data)
{
	int			lastchild_pid;
	int			wstatus;

	if (data->next == NULL && 
		(data->cmd_type == CD || data->cmd_type == EXPORT || data->cmd_type == UNSET || data->cmd_type == EXIT))
	{
		if (set_execdata(data) == 0)
			to_cmd(data);
		free_2d_array(data->cmdline);
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

void	set_cmd_type(t_execdata *data)
{
	while (data)
	{
		if (data->clst == NULL)
			data->cmd_type = NON_CMD;
		else if (ft_strncmp(data->clst->str, "echo", 10) == 0)
			data->cmd_type = ECHO;
		else if (ft_strncmp(data->clst->str, "cd", 10) == 0)
			data->cmd_type = CD;
		else if (ft_strncmp(data->clst->str, "pwd", 10) == 0)
			data->cmd_type = PWD;
		else if (ft_strncmp(data->clst->str, "export", 10) == 0)
			data->cmd_type = EXPORT;
		else if (ft_strncmp(data->clst->str, "unset", 10) == 0)
			data->cmd_type = UNSET;
		else if (ft_strncmp(data->clst->str, "env", 10) == 0)
			data->cmd_type = ENV;
		else if (ft_strncmp(data->clst->str, "exit", 10) == 0)
			data->cmd_type = EXIT;
		else
			data->cmd_type = OTHER;
		data = data->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_cmdlist	*clst;
	t_iolist	*iolst;
	t_envlist	*elst;
	t_execdata	*data;
	int			*status;
	t_envlist	*etmp;

/*
	data = NULL;
	//elst
	elst = NULL;
	elst = add_envlist(elst, "HOME", "/Users/ryojiroakiyama");
	elst = add_envlist(elst, "PATH", "/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands");
	elst = add_envlist(elst, "akiyama", "akiyama");
	//status
	status = (int *)malloc(sizeof(int));
	*status = 0;
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	data = add_execdata(data, status, ENV, clst, iolst, elst);
	execute_start(data);

	etmp = data->elst;

	//elst
	elst = etmp;
	//data
	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "unset");
	clst = add_cmdlist(clst, "akiyama");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	data = add_execdata(data, status, UNSET, clst, iolst, etmp);
	//execute
	execute_start(data);

	etmp = data->elst;

	//elst
	elst = etmp;
	//data
	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	data = add_execdata(data, status, ENV, clst, iolst, etmp);
	execute_start(data);
	free_data(data);
*/

	data = NULL;
	//elst
	elst = NULL;
	elst = add_envlist(elst, "HOME", "/Users/ryojiroakiyama");
	elst = add_envlist(elst, "PATH", "/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands");
	//status
	status = (int *)malloc(sizeof(int));
	*status = 0;
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "echo");
	clst = add_cmdlist(clst, "-n");
	clst = add_cmdlist(clst, "akiyama");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "<", -1);
	iolst = add_iolist(iolst, ELSE, "hello.txt", -1);
	iolst = add_iolist(iolst, OUT_REDIRECT, ">", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	data = add_execdata(data, status, ECHO, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "echo");
	clst = add_cmdlist(clst, " ryojiro");
	iolst = NULL;
	iolst = add_iolist(iolst, OUT_HERE_DOC, ">>", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	data = add_execdata(data, status, ECHO, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	data = add_execdata(data, status, ENV, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "cat");
	clst = add_cmdlist(clst, "-e");
	iolst = NULL;
	data = add_execdata(data, status, OTHER, clst, iolst, elst);
	//execute
	set_cmd_type(data);
	execute_start(data);
	free_data(data);

//	put_data(data);

//	system("leaks a.out");
	return (0);
}
