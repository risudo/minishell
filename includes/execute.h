#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"

//execution_start.c
void		execute_command(t_execdata *data);
int			execute_loop(t_execdata *data);
void		execute_start(t_execdata *data);

//execution_setdata.c
int			setdata_cmdline_redirect(t_execdata *data);
void		setdata_heredoc_cmdtype(t_execdata *data);

//command_*.c
void		builtin_echo(t_execdata *data);
void		builtin_cd(t_execdata *data);
void		builtin_pwd(t_execdata *data);
void		builtin_export(t_execdata *data);
void		builtin_unset(t_execdata *data);
void		builtin_env(t_execdata *data);
void		builtin_exit(t_execdata *data);
void		non_builtin(t_execdata *data);
void		no_command(t_execdata *data);

//execution_utils.c
int			ft_stat(char *pathname);
int			ft_dup2(int oldfd, int newfd);
int			ft_open(t_iolist *filenode, int flags, mode_t mode);
void		expansion_key_heredoc(char **line, \
				t_envlist *envlist, char *doll_ptr);

//env_functions.c
char		*ft_getenv(t_envlist *elst, char *search_key);
t_envlist	*ft_unsetenv(t_envlist *elst, char *rm_key);
void		ft_setenv(t_envlist *elst, char *new_key, \
						char *new_value, int append);

//execution_utils_list.c
char		**convert_envlist_2dchar(t_envlist *elst);
char		**convert_cmdlist_2dchar(t_cmdlist *clst);

//execution_utils_libft.c
long		ft_atol(char *str, bool *nonnum_check);
int			ft_strcmp(char *s1, char *s2);
void		free_2d_array(char **array);

//wrapper2.c
char		*ft_xstrjoin(char *str1, char *str2);
char		**ft_xsplit(char *src_str, char cut_char);
void		xclose(int fd);
int			xdup2(int oldfd, int newfd);
int			xdup(int oldfd);

//wrapper3.c
void		xwaitpid(pid_t pid, int *wstatus, int options);
void		xpipe(int *pipefd);
pid_t		xfork(void);

//minishell_loop.c
void		minishell_loop(char **envp);

#endif