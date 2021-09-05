#include "minishell.h"

/*
** Execute non built-in commands or no command.
*/

/*
** make list from split the PATH and join "/" + "command".
** example (command is cat)
** [0]/usr/local/bin/cat
** [1]/usr/bin/cat
** [2]/bin/cat
**  :
** finally return the list.
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
** search the valid executable file path from PATH env.
*/
static char	*cmdpath_from_pathenv(char *cmd, char *path_env)
{
	char		**pathlist;
	char		*cmd_path;
	size_t		i;

	pathlist = make_exec_pathlist(cmd, path_env);
	cmd_path = NULL;
	i = 0;
	while (pathlist && pathlist[i])
	{
		if (ft_stat(pathlist[i]) == IS_FILE)
		{
			cmd_path = ft_xstrdup(pathlist[i]);
			break ;
		}
		i++;
	}
	free_2d_array(pathlist);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	return (cmd_path);
}

/*
** verify if the file path is valid.
*/
static char	*cmdpath_from_direct(char *cmd)
{
	t_path_type	path_type;
	char		*cmd_path;

	cmd_path = NULL;
	path_type = ft_stat(cmd);
	if (path_type == UNKNOWN)
		perror(cmd);
	else if (path_type == IS_FILE)
		cmd_path = cmd;
	else if (path_type == IS_DIR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	return (cmd_path);
}

void	non_builtin(t_execdata *data)
{
	char	*cmd_path;

	*(data->status) = 0;
	if (ft_strchr(data->cmdline[0], '/'))
		cmd_path = cmdpath_from_direct(data->cmdline[0]);
	else
		cmd_path = cmdpath_from_pathenv(data->cmdline[0], \
								ft_getenv(data->elst, "PATH"));
	if (!cmd_path)
		exit(127);
	if (execve(cmd_path, data->cmdline, \
		convert_envlist_2dchar(data->elst)) == -1)
		perror(data->cmdline[0]);
	*(data->status) = 126;
}

