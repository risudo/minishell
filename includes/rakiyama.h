#ifndef RAKIYAMA_H
# define RAKIYAMA_H

# include "minishell.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

//execute_start.c
void	execute_command(t_execdata *data);
int		execute_loop(t_execdata *data);
void	execute_start(t_execdata *data);

//setdata.c
int		setdata_cmdline_redirect(t_execdata *data);
int		is_cmd_type(t_cmdlist *clst);
enum e_cmd	get_here_doc(char *limiter);
void	setdata_heredoc_cmdtype(t_execdata *data);

//command.c
void	builtin_echo(t_execdata *data);
void	builtin_cd(t_execdata *data);
void	builtin_pwd(t_execdata *data);
void	builtin_export(t_execdata *data);
void	builtin_unset(t_execdata *data);
void	builtin_env(t_execdata *data);
void	builtin_exit(t_execdata *data);
void	non_builtin(t_execdata *data);
void	non_command(t_execdata *data);

//utils_execute.c
int	ft_stat(char *pathname);
int	ft_dup2(int oldfd, int newfd);
int	ft_open(char *filepath, int flags, mode_t mode);

//utils_env.c
char	*ft_getenv(t_envlist *elst, char *search_key);
t_envlist	*ft_unsetenv(t_envlist *elst, char *rm_key);
void	ft_setenv(t_envlist *elst, char *new_key, char *new_value, int append);

//utils_list.c
size_t	envlist_size(t_envlist *head);
size_t	cmdlist_size(t_cmdlist *head);
char	**convert_envlist_2dchar(t_envlist *elst);
char	**convert_cmdlist_2dchar(t_cmdlist *clst);

//utils_libft.c
long	ft_atolong(char *str, bool *nonnum_check);
int		ft_strcmp(char *s1, char *s2);
void	free_2d_array(char **array);

//wrapper2.c
char	*ft_xstrjoin(char *str1, char *str2);
char	**ft_xsplit(char *src_str, char cut_char);
void	xclose(int fd);
int		xdup2(int oldfd, int newfd);
int		xdup(int oldfd);
void	xwaitpid(pid_t pid, int *wstatus, int options);
void	xpipe(int *pipefd);
pid_t	xfork(void);

#endif