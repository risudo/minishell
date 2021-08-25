#include "rakiyama.h"

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

int	ft_strcmp(char *s1, char *s2)
{
	if (s1 == NULL)
		return (-1);
	if (s2 == NULL)
		return (1);
	while (1)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		if (*s1 == 0)
			return (0);
		s1++;
		s2++;
	}
}

size_t	envlist_size(t_envlist *head)
{
	t_envlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

size_t	cmdlist_size(t_cmdlist *head)
{
	t_cmdlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
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
	size_t	i;

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
	size_t		i;

	if (ft_stat(cmd))//replace back
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

char	*ft_getenv(t_envlist *elst, char *search_key)
{
	while (elst)
	{
		if (ft_strcmp(elst->key, search_key) == 0)
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
		if (ft_strcmp(elst->key, rm_key) == 0)
		{
			if (prev == NULL)
				head = elst->next;
			else
				prev->next = elst->next;
			free(elst->key);
			free(elst->value);
			free(elst);
			break ;
		}
		prev = elst;
		elst = elst->next;
	}
	return (head);
}

t_envlist	*get_elst_node(char *new_key, char *new_value)
{
	t_envlist	*new_elst;

	new_elst = ft_xcalloc(1, sizeof(*new_elst));
	new_elst->key = new_key;
	new_elst->value = new_value;
	return (new_elst);
}

void	ft_setenv(t_envlist *elst, char *new_key, char *new_value, int append)
{
	char		*add_value;

	while(1)
	{
		if (ft_strcmp(elst->key, new_key) == 0)
		{
			if (new_value)
			{
				if (append)
					add_value = ft_xstrjoin(elst->value, new_value);
				else
					add_value = ft_xstrdup(new_value);
				free(new_value);
				free(elst->value);
				elst->value = add_value;
			}
			free(new_key);
			return ;
		}
		if (elst->next == NULL)
			break ;
		elst = elst->next;
	}
	elst->next = get_elst_node(new_key, new_value);
}

/*
convert t_envlist type list -> char type 2d array
*/
char	**convert_envlist_2dchar(t_envlist *elst)
{
	char 		**array;
	t_envlist	*move;
	size_t		cnt;
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
	*(data->status) = 126;//is ok?
}

void	ft_echo(t_execdata *data)
{
	int		option;
	size_t	arg_i;

	option = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] && ft_strcmp(data->cmdline[arg_i], "-n") == 0)
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

void	setenv_curpwd_oldpwd(t_envlist *head)
{
	char	*pwd_key;
	char	*pwd_value;

	pwd_key = ft_strdup("OLDPWD");
	pwd_value = ft_strjoin(ft_getenv(head, "PWD"), 0);
	if (pwd_key)
		ft_setenv(head, pwd_key, pwd_value, 0);
	else
	{
		free(pwd_key);
		free(pwd_value);
	}
	pwd_key = ft_strdup("PWD");
	pwd_value = getcwd(NULL, 0);
	if (pwd_key)
		ft_setenv(head, pwd_key, pwd_value, 0);
	else
	{
		free(pwd_key);
		free(pwd_value);
	}
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
	if (chdir(path) == -1)//if path="" what happen
	{
		perror("cd");
		*(data->status) = 1;
		return ;
	}
	setenv_curpwd_oldpwd(data->elst);
	*(data->status) = 0;
}

void	ft_pwd(t_execdata *data)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);//if no authority to directory what happen
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

ssize_t	check_name_rule(char *src_str)
{
	ssize_t	i;

	i = 0;
	while(src_str[i])
	{
		if (0 < i && ((src_str[i] == '+' && src_str[i + 1] == '=') || src_str[i] == '='))
			break ;
		if (!(src_str[i] == '_' || ft_isalnum(src_str[i])) || ft_isdigit(src_str[0]))
			return (-1);
		i++;
	}
	return(i);
}

void	to_setenv(t_envlist *head, char *src_str, size_t i)
{
	char	*key;
	char	*value;
	int		mode;

	if (src_str[i] == '+')
	{
		key = ft_xsubstr(src_str, 0, i);
		value = ft_xsubstr(src_str, i + 2, ft_strlen(src_str) - i - 2);
		mode = 1;
	}
	else if (src_str[i] == '=')
	{
		key = ft_xsubstr(src_str, 0, i);
		value = ft_xsubstr(src_str, i + 1, ft_strlen(src_str) - i - 1);
		mode = 0;
	}
	else
	{
		key = ft_xstrdup(src_str);
		value = NULL;
		mode = 0;
	}
	ft_setenv(head, key, value, mode);
}

void	put_env_asciiorder(t_envlist *head, t_envlist *min_node)
{
	t_envlist	*put_node;
	t_envlist	*move;

	put_node = NULL;
	move = head;
	while (move)
	{
		if (min_node == NULL || ft_strcmp(min_node->key, move->key) < 0)
		{
			if (put_node == NULL)
				put_node = move;
			else if (ft_strcmp(move->key, put_node->key) < 0)
				put_node = move;
		}
		move = move->next;
	}
	if (put_node)
	{
		printf("declare -x %s=\"%s\"\n", put_node->key, put_node->value);
		put_env_asciiorder(head, put_node);
	}
}

void	ft_export(t_execdata *data)
{
	size_t	arg_i;
	ssize_t	split_point;

	*(data->status) = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] == NULL)
		put_env_asciiorder(data->elst, NULL);
	else
	{
		while (data->cmdline[arg_i])
		{
			split_point = check_name_rule(data->cmdline[arg_i]);
			if (split_point != -1)
				to_setenv(data->elst, data->cmdline[arg_i], split_point);
			else
			{
				ft_putstr_fd("minishell: export: ", STDERR_FILENO);
				ft_putstr_fd(data->cmdline[arg_i], STDERR_FILENO);
				ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
				*(data->status) = 1;
			}
			arg_i++;
		}
	}
}

void	ft_unset(t_execdata *data)
{
	size_t	i;

	i = 1;
	while (data->cmdline[i])
	{
		ft_unsetenv(data->elst, data->cmdline[i]);
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
		if (move->value)
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
	if (mode == 0)
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
	size_t		cnt;

	cnt = cmdlist_size(clst);
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
			data->in_fd = ft_open(data->in_fd, move->next->str, O_RDONLY, 0);//option ok?
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

void	execute_command(t_execdata *data)
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
				execute_command(data);
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

t_execdata	*add_execdata(t_execdata *data, int *s, t_cmdlist *clst, t_iolist *iolst, t_envlist *elst)
{
	t_execdata	*tmp;
	t_execdata	*move;

	tmp = (t_execdata *)malloc(sizeof(t_execdata));
	tmp->cmdline = NULL;
	tmp->in_fd = STDIN_FILENO;
	tmp->out_fd = STDOUT_FILENO;
	tmp->status = s;
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

void	free_data(t_execdata *data, int status_free, int elst_free)
{
	void		*tmp;
	t_envlist	*elst_tmp;

	if (status_free)
		free(data->status);
	while (elst_free && data->elst)
	{
		elst_tmp = data->elst;
		data->elst = data->elst->next;
		free(elst_tmp->key);
		free(elst_tmp->value);
		free(elst_tmp);
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

void	set_cmd_type(t_execdata *data)
{
	while (data)
	{
		if (data->clst == NULL)
			data->cmd_type = NON_CMD;
		else if (ft_strcmp(data->clst->str, "echo") == 0)
			data->cmd_type = ECHO;
		else if (ft_strcmp(data->clst->str, "cd") == 0)
			data->cmd_type = CD;
		else if (ft_strcmp(data->clst->str, "pwd") == 0)
			data->cmd_type = PWD;
		else if (ft_strcmp(data->clst->str, "export") == 0)
			data->cmd_type = EXPORT;
		else if (ft_strcmp(data->clst->str, "unset") == 0)
			data->cmd_type = UNSET;
		else if (ft_strcmp(data->clst->str, "env") == 0)
			data->cmd_type = ENV;
		else if (ft_strcmp(data->clst->str, "exit") == 0)
			data->cmd_type = EXIT;
		else
			data->cmd_type = OTHER;
		data = data->next;
	}
}

void	execute_start(t_execdata *data)
{
	int			lastchild_pid;
	int			wstatus;

	set_cmd_type(data);
	if (data->next == NULL && 
		(data->cmd_type == CD || (data->cmd_type == EXPORT && data->clst->next) || data->cmd_type == UNSET || data->cmd_type == EXIT))
	{
		if (set_execdata(data) == 0)
			execute_command(data);
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

int	main(int ac, char **av, char **envp)
{
	t_cmdlist	*clst;
	t_iolist	*iolst;
	t_envlist	*elst;
	t_execdata	*data;
	int			*status;
	t_envlist	*etmp;
	int			exit_status;


	data = NULL;
	//elst
	elst = NULL;
	elst = add_envlist(elst, ft_strdup("HOME"), ft_strdup("/Users/ryojiroakiyama"));
	elst = add_envlist(elst, ft_strdup("PATH"), ft_strdup("/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands"));
	elst = add_envlist(elst, ft_strdup("EEE"), ft_strdup("aaaaaaai"));
	elst = add_envlist(elst, ft_strdup("PWD"), ft_strdup("/Users/ryojiroakiyama/Desktop/icloud/42_cursus/minishell/minishell/rakiyama"));
	elst = add_envlist(elst, ft_strdup("RYOJIRO"), ft_strdup("ryojiro"));
	elst = add_envlist(elst, ft_strdup("AKIYAMA"), ft_strdup("akiyama"));
	//status
	status = (int *)malloc(sizeof(int));
	*status = 0;
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	//elst
	//status
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "cd");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	//elst
	//status
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	//elst
	//status
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "export");
	clst = add_cmdlist(clst, "EEE+=uuuuuuuui");	
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	//elst
	//status
	//data
	clst = NULL;
	clst = add_cmdlist(clst, "export");	
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 1, 1);

/*
	clst = NULL;
	clst = add_cmdlist(clst, "echo");
	clst = add_cmdlist(clst, " ryojiro");
	iolst = NULL;
	iolst = add_iolist(iolst, OUT_HERE_DOC, ">>", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	data = add_execdata(data, status, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "export");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);

	clst = NULL;
	clst = add_cmdlist(clst, "cat");
	clst = add_cmdlist(clst, "-e");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	//execute
	execute_start(data);
	exit_status = *(data->status);
	free_data(data);
*/

//	put_data(data);

	if (system("leaks a.out > /dev/null"))
		system("leaks a.out");
	return (exit_status);
}
