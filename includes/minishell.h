#ifndef MINISHELL_H
# define MINISHELL_H

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
# include "../libft/libft.h"
# include "struct.h"
# include "parse.h"
# include "execute.h"

extern unsigned char	g_status;

t_execdata	*parse_cmd(char *command, t_envlist *envlist);
t_envlist	*create_envlist(char **envp);

#endif