/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_nonbuilin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:11:29 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Execute non built-in commands.
*/

#define CMD_NO "command not found"
#define PEM_NO "Permission denied"
#define DIR_NO "is a directory"
#define FILE_DIR_NO "No such file or directory"

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
	int		i;

	if (!path_env)
		return (NULL);
	pathlist = xft_split(path_env, ':');
	i = 0;
	while (pathlist[i])
	{
		tmp = pathlist[i];
		pathlist[i] = xft_strjoin(pathlist[i], "/");
		free(tmp);
		tmp = pathlist[i];
		pathlist[i] = xft_strjoin(pathlist[i], cmd);
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
	t_path_type	path_type;
	int			i;

	pathlist = make_exec_pathlist(cmd, path_env);
	cmd_path = NULL;
	i = -1;
	while (pathlist && pathlist[++i])
	{
		path_type = ft_stat(pathlist[i]);
		if (path_type == EXECUTABLE)
		{
			cmd_path = xft_strdup(pathlist[i]);
			break ;
		}
		else if (path_type == UN_EXECUTABLE)
			g_status = 126;
	}
	free_2d_array(pathlist);
	if (!cmd_path && g_status == 126)
		ft_puterror(cmd, PEM_NO, NULL);
	else if (!cmd_path)
		ft_puterror(cmd, CMD_NO, NULL), g_status = 127;
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
	{
		ft_perror(cmd);
		if (errno == ENOENT)
			g_status = 127;
		else
			g_status = 126;
	}
	else if (path_type == EXECUTABLE)
		cmd_path = cmd;
	else if (path_type == UN_EXECUTABLE)
		ft_puterror(cmd, PEM_NO, NULL), g_status = 126;
	else if (path_type == IS_DIR)
		ft_puterror(cmd, DIR_NO, NULL), g_status = 126;
	else
		ft_puterror(cmd, FILE_DIR_NO, NULL), g_status = 127;
	return (cmd_path);
}

void	non_builtin(t_execdata *data)
{
	char	*cmd_path;

	g_status = 0;
	if (ft_strchr(data->cmdline[0], '/'))
		cmd_path = cmdpath_from_direct(data->cmdline[0]);
	else
		cmd_path = cmdpath_from_pathenv(data->cmdline[0], \
								ft_getenv(data->elst, "PATH"));
	if (!cmd_path)
		exit(g_status);
	if (execve(cmd_path, data->cmdline, \
		convert_envlist_2dchar(data->elst)) == -1)
		ft_perror(data->cmdline[0]);
	g_status = 126;
}
