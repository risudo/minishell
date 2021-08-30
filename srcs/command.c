# include "execute.h"

/*
** all command functions
*/

//echo
void	builtin_echo(t_execdata *data)
{
	int		option;
	size_t	arg_i;

	option = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] && ft_strcmp(data->cmdline[arg_i], "-n") == 0 && arg_i++)
		option++;
	while (data->cmdline[arg_i])
	{
		printf("%s", data->cmdline[arg_i]);
		if (data->cmdline[arg_i + 1] != NULL)
			printf(" ");
		arg_i++;
	}
	if (option == 0)
		printf("\n");
	*(data->status) = 0;
}

//cd
void	builtin_cd(t_execdata *data)
{
	ft_setenv(data->elst, ft_xstrdup("OLDPWD"), getcwd(NULL, 0), 0);
	if (data->cmdline[1] && data->cmdline[1][0] && chdir(data->cmdline[1]) == -1)
	{
		perror("cd");
		*(data->status) = 1;
		return ;
	}
	if (ft_getenv(data->elst, "PWD"))
		ft_setenv(data->elst, ft_xstrdup("PWD"), getcwd(NULL, 0), 0);
	*(data->status) = 0;
}

//pwd
void	builtin_pwd(t_execdata *data)
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

//export
static ssize_t	check_name_rule(char *src_str)
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

static void	to_setenv(t_envlist *head, char *src_str, size_t i)
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

static void	put_env_asciiorder(t_envlist *head, t_envlist *min_node)
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

void	builtin_export(t_execdata *data)
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

//unset
void	builtin_unset(t_execdata *data)
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

//env
void	builtin_env(t_execdata *data)
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

//exit
void	builtin_exit(t_execdata *data)
{
	bool	nonnum_check;

	if (data->cmdline[1] == NULL)
		exit(0);
	if (data->cmdline[2])
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n", STDERR_FILENO);
		*(data->status) = 1;
		return ;
	}
	*(data->status) = (unsigned char)ft_atol(data->cmdline[1], &nonnum_check);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (nonnum_check)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(data->cmdline[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		*(data->status) = 255;
	}
	exit(*(data->status));
}

//non_buitin
/*
split PATH and join "/" + "command"
example (command is cat)
/usr/local/bin/cat
/usr/bin/cat
/bin/cat
:
finally return the list
*/
static char	**make_exec_pathlist(char *cmd, char *path_env)
{
	char	**pathlist;
	char	*tmp;
	size_t	i;

	if (!path_env)
		return (NULL);
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
static char	*set_cmd_path(char *cmd, char *path_env)
{
	char		**pathlist;
	char		*cmd_path;
	size_t		i;

	if (ft_strchr(cmd, '/') && ft_stat(cmd))
		return (cmd);
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

void	non_builtin(t_execdata *data)
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

//non_command
void	non_command(t_execdata *data)
{
	*(data->status) = 0;
}