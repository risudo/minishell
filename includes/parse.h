/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:14:54 by rakiyama          #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"
# include "../libft/libft.h"
# include <stdbool.h>
# include <stdio.h>

// wrapper
void		*xft_calloc(size_t count, size_t size);
char		*xft_substr(char const *s, unsigned int start, size_t len);
char		*xft_strdup(const char *src);
char		*xft_itoa(int n);
void		xsignal(int sig, void f(int));

// tokenize
t_token		*tokenize_cmd(char *cmd, bool *err);
int			parse_tokenlist(t_token *list);
int			split_operater(t_token *list);
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
void		clear_execdata(t_execdata *data);
void		clear_tokenlist(t_token *list);
void		clear_envlist(t_envlist *envlist);

// check syntax
t_execdata	*check_syntax(t_execdata *data);
void		put_syntax_error(char *str);

// expansion
void		expand_cmdlist(t_cmdlist **clist, t_envlist *envlist);
char		*get_quot_flag(char *str);
char		*get_removed_endflag(char **quot, char flag);
size_t		ft_strlen_excluded_quot(char *str, char *quot);
size_t		get_space_idx(t_cmdlist *clist);
char		*ft_strdoll(char *s, char *clist, char *head);

// expansion_utils
bool		is_delimiter(char c);
char		*ft_strjoin_three(char *str1, char *str2, char *str3);
bool		is_delimiter_quot(char c, char flag);
int			delone_cmdlist(t_cmdlist **cur, t_cmdlist *prev, t_cmdlist **head);

// test
void		put_execdata(t_execdata *data);
void		put_tokenlist(t_token *list);
void		put_cmdlist(t_cmdlist *list);
void		put_iolist(t_iolist *list);
void		put_envlist(t_envlist *envlist);

#endif
