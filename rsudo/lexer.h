#ifndef LEXER_H
# define LEXER_H

# include "../includes/minishell.h"
# include "../libft/libft.h"
# include <stdbool.h>
# include <stdio.h>

typedef struct s_execarg
{
	t_envlist	*envlist;
	int			*status;
}	t_execarg;

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

// *test
void		put_execdata(t_execdata *data);
void		put_tokenlist(t_token *list);
void		put_cmdlist(t_cmdlist *list);
void		put_iolist(t_iolist *list);
void		put_envlist(t_envlist *envlist);

#endif
