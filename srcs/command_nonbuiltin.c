#include "execute.h"

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
	while (pathlist && pathlist[i])
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
	if (execve(cmd_path, data->cmdline, \
		convert_envlist_2dchar(data->elst)) == -1)
		perror(data->cmdline[0]);
	*(data->status) = 126;
}

//non_command
void	non_command(t_execdata *data)
{
	*(data->status) = 0;
}
