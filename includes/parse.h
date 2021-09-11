#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"
# include "../libft/libft.h"
# include <stdbool.h>
# include <stdio.h>

// wrapper
void		*ft_xcalloc(size_t count, size_t size);
char		*ft_xsubstr(char const *s, unsigned int start, size_t len);
char		*ft_xstrdup(const char *src);
char		*ft_xitoa(int n);

// tokenize
t_token		*tokenize_cmd_by_space(char *cmd, bool *err);
int			parse_tokenlist(t_token *list);
int			split_operater(t_token *list);
void		clear_execdata(t_execdata *data);
t_token		*join_redirect(t_token *list);
bool		is_consecutive_redirect(t_token *list);
bool		is_space_after_quot(t_token *list);
bool		ft_isspace(char c);
t_quottype	get_flag_quot(char *cmd, t_quottype flag_quot);

// parse tokenlist
int			parse_tokenlist(t_token *list);
void		set_special_c(t_token *list);

// execdata
t_execdata	*create_execdata(t_token *tokenlist,
				t_envlist *envlist);
t_iolist	*new_iolst(t_iolist *cur, t_token *token);
t_cmdlist	*new_clst(t_cmdlist *cur, t_token *token);
bool		is_cmd(t_token *start);
t_iolist	*delimit_fd(t_iolist *cur);

// clear list
void		clear_tokenlist(t_token *list);
void		clear_envlist(t_envlist *envlist);

// check syntax
t_execdata	*check_syntax(t_execdata *data);
void		put_syntax_error(char *str);

// expansion
void		serch_env_cmdlist(t_cmdlist *clist, t_envlist *envlist);
char		*get_quot_flag(char *str);
char		*get_removed_endflag(char **quot, char flag);
size_t		ft_strlen_excluded_quot(char *str, char *quot);
bool		is_delimiter(char c);
char		*ft_strjoin_three(char *str1, char *str2, char *str3);
bool		is_env_iolist(char flag, t_iolist *prev);
size_t		get_space_idx(t_cmdlist *clist);

// expansion_utils
bool		is_delimiter(char c);
char		*ft_strjoin_three(char *str1, char *str2, char *str3);
bool		is_delimiter_quot(char c, char flag);

// test
void		put_execdata(t_execdata *data);
void		put_tokenlist(t_token *list);
void		put_cmdlist(t_cmdlist *list);
void		put_iolist(t_iolist *list);
void		put_envlist(t_envlist *envlist);

#endif
