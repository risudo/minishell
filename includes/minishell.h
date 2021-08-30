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

typedef enum e_quottype
{
	DEFAULT,
	D_QUOT,
	S_QUOT,
	END_S_QUOT,
	END_D_QUOT,
}	t_quottype;

typedef enum e_special_c
{
	IN_REDIRECT,
	OUT_REDIRECT,
	IN_HERE_DOC,
	OUT_HERE_DOC,
	PIPE,
	ELSE
}	t_special_c;

typedef enum e_cmd
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	OTHER,
	NON_CMD,
	CMD_NUM
}	t_cmd;

typedef enum e_pipefd
{
	READ,
	WRITE,
	PIPEFD_NUM
}	t_pipefd;

typedef struct s_token
{
	char			*str;
	t_quottype		type;
	t_special_c		special;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_iolist
{
	t_special_c		c_type;
	char			*str;
	char			*quot;
	int				here_doc_fd;
	struct s_iolist	*next;
}	t_iolist;

typedef struct s_cmdlist
{
	char				*str;
	char				*quot;
	struct s_cmdlist	*next;
}	t_cmdlist;

typedef struct s_envlist
{
	char				*key;
	char				*value;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_execdata
{
	char				**cmdline;
	int					in_fd;
	int					out_fd;
	int					ori_stdin;
	int					ori_stdout;
	int					ori_stderr;
	unsigned char		*status;
	int					pipefd[PIPEFD_NUM];
	t_cmd				cmd_type;
	t_cmdlist			*clst;
	t_iolist			*iolst;
	t_envlist			*elst;
	struct s_execdata	*next;
}	t_execdata;

# include "parse.h"
# include "execute.h"
t_execdata	*parse_cmd(char *command, t_envlist *envlist, unsigned char *status);
t_envlist	*create_envlist(char **envp);

#endif