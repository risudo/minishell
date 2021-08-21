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
/*
void	to_cmd(t_execdata *edata)
{
	if (data->cmdline == NULL)
		return ;
	else if (ft_strncmp(data->cmdline[0], "echo", 10) == 0)
		ft_echo(data);
	else if (ft_strncmp(data->cmdline[0], "cd", 10) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->cmdline[0], "pwd", 10) == 0)
		ft_pwd(data);
	else if (ft_strncmp(data->cmdline[0], "export", 10) == 0)
		ft_export(data);
	else if (ft_strncmp(data->cmdline[0], "unset", 10) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->cmdline[0], "env", 10) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->cmdline[0], "exit", 10) == 0)
		ft_exit(data);
	else
		ft_execve(data);
}
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

//from rsudo
void	*ft_xcalloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
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
	return ;
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

void	ft_execve(t_execdata *edata)
{
	char	*cmd_path;

	*(edata->status) = 0;
	cmd_path = set_cmd_path(edata->cmdline[0], ft_getenv(edata->elst, "PATH"));
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(edata->cmdline[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (execve(cmd_path, edata->cmdline, convert_envlist_2dchar(edata->elst)) == -1)
		perror(edata->cmdline[0]);
	*(edata->status) = 126;
}

void	ft_echo(t_execdata *edata)
{
	int	option;
	int	arg_i;

	option = 0;
	arg_i = 1;
	if (edata->cmdline[arg_i] && ft_strncmp(edata->cmdline[arg_i], "-n", 5) == 0)
	{
		option++;
		arg_i++;
	}
	while (edata->cmdline[arg_i])
	{
		printf("%s", edata->cmdline[arg_i]);
		arg_i++;
	}
	if (option == 0)
		printf("\n");
	*(edata->status) = 0;
}

void	ft_cd(t_execdata *edata)
{
	char	*path;

	if (edata->cmdline[1] == NULL)
	{
		path = ft_getenv(edata->elst, "HOME");
		if (path == NULL)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			*(edata->status) = 1;
			return ;
		}
	}
	else
		path = edata->cmdline[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		*(edata->status) = 1;
		return ;
	}
	*(edata->status) = 0;//change env? old pwd pwd
}

void	ft_pwd(t_execdata *edata)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		perror("pwd");
		*(edata->status) = 1;
	}
	else
	{
		printf("%s\n", pathname);
		*(edata->status) = 0;
	}
	free(pathname);
}

void	ft_export(t_execdata *edata)
{
//	envlist_add_back();
	return ;
}

void	ft_unset(t_execdata *edata)
{
	int i;

	i = 1;
	while (edata->cmdline[i])
	{
		ft_unsetenv(edata->elst, edata->cmdline[1]);
		i++;
	}
	*(edata->status) = 0;
}

void	ft_env(t_execdata *edata)
{
	t_envlist	*move;

	move = edata->elst;
	while(move)
	{
		printf("%s=%s\n", move->key, move->value);
		move = move->next;
	}
	*(edata->status) = 0;
}

void	ft_exit(t_execdata *edata)
{
	exit(0);
}

void	ft_non_cmd(t_execdata *edata)
{
	*(edata->status) = 0;
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
edata->cmdline
edata->in_fd
edata->out_fd
*/
int	set_execdata(t_execdata *edata)
{
	t_iolist	*move;

	edata->cmdline = convert_cmdlist_2dchar(edata->clst);
	move = edata->iolst;
	while(move)
	{
		if (move->c_type == IN_REDIRECT)
			edata->in_fd = ft_open(edata->in_fd, move->str, O_RDONLY, -1);//option ok?
		else if (move->c_type == IN_HERE_DOC)
			edata->in_fd = move->here_doc_fd;
		else if (move->c_type == OUT_REDIRECT)
			edata->out_fd = ft_open(edata->out_fd, move->str, O_RDWR | O_TRUNC | O_CREAT, S_IREAD | S_IWRITE);
		else if (move->c_type ==  OUT_HERE_DOC)
			edata->out_fd = ft_open(edata->out_fd, move->str, O_RDWR | O_APPEND | O_CREAT, S_IREAD | S_IWRITE);
		if (edata->in_fd < 0 || edata->out_fd < 0)
		{
			*(edata->status) = 1;
			return (-1);
		}
		move = move->next;
	}
	return (0);
}

int	dup_io(t_execdata *edata)
{
	if (edata->in_fd != STDIN_FILENO)
	{
		xdup2(edata->in_fd, STDIN_FILENO);
		xclose(edata->in_fd);
	}
	if (edata->out_fd != STDOUT_FILENO)
	{
		xdup2(edata->out_fd, STDOUT_FILENO);
		xclose(edata->out_fd);
	}
	return (0);
}

void	to_cmd(t_execdata *edata)
{
	void	(*cmd_func[CMD_NUM])(t_execdata *edata);

	cmd_func[ECHO] = ft_echo;
	cmd_func[CD] = ft_cd;
	cmd_func[PWD] = ft_pwd;
	cmd_func[EXPORT] = ft_export;
	cmd_func[UNSET] = ft_unset;
	cmd_func[ENV] = ft_env;
	cmd_func[EXIT] = ft_exit;
	cmd_func[OTHER] = ft_execve;
	cmd_func[NON_CMD] = ft_non_cmd;
	cmd_func[edata->cmd_type](edata);
}

/*
if pipe exists in input,
this loop execute each command and connect those input and output
*/
int	execute_loop(t_execdata *edata)
{
	int	pid;

	while (edata)
	{
		xpipe(edata->pipefd);
		pid = xfork();
		if (pid == 0)
		{
			xclose(edata->pipefd[READ]);
			if (edata->next != NULL)
				edata->out_fd = edata->pipefd[WRITE];
			if (set_execdata(edata) == 0 && dup_io(edata) == 0)
				to_cmd(edata);
			exit(*(edata->status));
		}
		else
		{
			xclose(edata->pipefd[WRITE]);
			xdup2(edata->pipefd[READ], STDIN_FILENO);
			xclose(edata->pipefd[READ]);
		}
		edata = edata->next;
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

t_execdata	*add_execdata(t_execdata *edata, int *s, int c, t_cmdlist *clst, t_iolist *iolst, t_envlist *elst)
{
	t_execdata	*tmp;
	t_execdata	*move;

	tmp = (t_execdata *)malloc(sizeof(t_execdata));
	tmp->cmdline = NULL;
	tmp->in_fd = STDIN_FILENO;
	tmp->out_fd = STDOUT_FILENO;
	tmp->status = s;
	tmp->cmd_type = c;
	tmp->clst = clst;
	tmp->iolst = iolst;
	tmp->elst = elst;
	tmp->next = NULL;
	if (edata == NULL)
		return(tmp);
	move = edata;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (edata);
}

void	put_edata(t_execdata *edata)
{
	t_cmdlist	*ctmp;
	t_iolist	*iotmp;
	t_envlist	*etmp;

	while(edata)
	{
		printf("--------------------------------------------\n");
		printf("status:%d\n", *(edata->status));
		ctmp = edata->clst;
		while (ctmp)
		{
			printf("cmdlist:%s\n", ctmp->str);
			ctmp = ctmp->next;
		}
		iotmp = edata->iolst;
		while (iotmp)
		{
			printf("iolist:%s\n", iotmp->str);
			iotmp = iotmp->next;
		}
		etmp = edata->elst;
		while (etmp)
		{
			printf("env key:%s\n", etmp->key);
			printf("env value:%s\n", etmp->value);
			etmp = etmp->next;
		}
		edata = edata->next;
	}
}

void	free_edata(t_execdata *edata)
{
	void		*tmp;

	free(edata->status);
	while (edata->elst)
	{
		tmp = edata->elst;
		edata->elst = edata->elst->next;
		free(tmp);
	}
	while(edata)
	{
		while (edata->clst)
		{
			tmp = edata->clst;
			edata->clst = edata->clst->next;
			free(tmp);
		}
		while (edata->iolst)
		{
			tmp = edata->iolst;
			edata->iolst = edata->iolst->next;
			free(tmp);
		}
		tmp = edata;
		edata = edata->next;
		free(tmp);
	}
}

void	execute_start(t_execdata *edata)
{
	int			lastchild_pid;
	int			wstatus;

	if (edata->next == NULL && 
		(edata->cmd_type == CD || edata->cmd_type == EXPORT || edata->cmd_type == UNSET || edata->cmd_type == EXIT))
	{
		if (set_execdata(edata) == 0)
			to_cmd(edata);
		free_2d_array(edata->cmdline);
	}
	else
	{
		lastchild_pid = execute_loop(edata);
		xwaitpid(lastchild_pid, &wstatus, 0);
		*(edata->status) = WEXITSTATUS(wstatus);//confirm WIFEXITED and WIFSIGNALED etc..
		while(edata->next)
		{
			xwaitpid(0, NULL, 0);
			edata = edata->next;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_cmdlist	*clst;
	t_iolist	*iolst;
	t_envlist	*elst;
	t_execdata	*edata;
	int			*status;
	t_envlist	*etmp;

/*
	edata = NULL;
	//elst
	elst = NULL;
	elst = add_envlist(elst, "HOME", "/Users/ryojiroakiyama");
	elst = add_envlist(elst, "PATH", "/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands");
	elst = add_envlist(elst, "akiyama", "akiyama");
	//status
	status = (int *)malloc(sizeof(int));
	*status = 0;
	//edata
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	edata = add_execdata(edata, status, ENV, clst, iolst, elst);
	execute_start(edata);

	etmp = edata->elst;

	//elst
	elst = etmp;
	//edata
	edata = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "unset");
	clst = add_cmdlist(clst, "akiyama");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	edata = add_execdata(edata, status, UNSET, clst, iolst, etmp);
	//execute
	execute_start(edata);

	etmp = edata->elst;

	//elst
	elst = etmp;
	//edata
	edata = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	edata = add_execdata(edata, status, ENV, clst, iolst, etmp);
	execute_start(edata);
	free_edata(edata);
*/

	edata = NULL;
	//elst
	elst = NULL;
	elst = add_envlist(elst, "HOME", "/Users/ryojiroakiyama");
	elst = add_envlist(elst, "PATH", "/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands");
	//status
	status = (int *)malloc(sizeof(int));
	*status = 0;
	//edata
	clst = NULL;
	clst = add_cmdlist(clst, "cat");
	clst = add_cmdlist(clst, "-e");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	iolst = add_iolist(iolst, OUT_REDIRECT, "outfile", -1);
	edata = add_execdata(edata, status, OTHER, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "echo");
	clst = add_cmdlist(clst, "-n");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "hello.txt", -1);
	edata = add_execdata(edata, status, ECHO, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	edata = add_execdata(edata, status, ENV, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "cat");
	clst = add_cmdlist(clst, "-e");
	iolst = NULL;
	edata = add_execdata(edata, status, OTHER, clst, iolst, elst);
	//execute
	execute_start(edata);
	free_edata(edata);

//	put_edata(edata);

//	system("leaks a.out");
	return (0);
}
