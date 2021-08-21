#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"
# include "../libft/libft.h"
# include <stdbool.h>
# include <stdio.h>

typedef struct s_execarg
{
	t_envlist	*envlist;
	int			*status;
}	t_execarg;

void		*ft_xcalloc(size_t count, size_t size);
char		*ft_xsubstr(char const *s, unsigned int start, size_t len);
char		*ft_xstrdup(const char *src);
void		xfree(void *ptr);

t_token		*tokenize_cmd_by_space(char *cmd);
int			parse_tokenlist(t_token *list);
int			split_operater(t_token *list);
void		clear_tokenlist(t_token *list);
t_envlist	*create_envlist(char **envp);
void		clear_envlist(t_envlist *envlist);
t_execdata	*create_execdata(t_token *tokenlist, t_envlist *envlist);
t_iolist	*get_iolst(t_token *start, t_token *cur_token);
t_cmdlist	*get_clst(t_token *start, t_token *cur_token);
void		clear_execdata(t_execdata *data);
t_token		*join_redirect(t_token *list);
bool		is_consecutive_redirect(t_token *list);
t_token		*trim_list(t_token *list);
bool		is_space_after_quot(t_token *list);
void		set_special_c(t_token *list);


// *test
void		put_execdata(t_execdata *data);
void		put_tokenlist(t_token *list);
void		put_cmdlist(t_cmdlist *list);
void		put_iolist(t_iolist *list);
void		put_envlist(t_envlist *envlist);

#endif
