#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parse.h"

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

enum e_cmd
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
};

enum e_pipefd
{
	READ,
	WRITE,
	PIPEFD_NUM
};

typedef struct s_iolist
{
	t_special_c		c_type;
	char			*str;
	int				here_doc_fd;
	char			*quot;
	struct s_iolist	*next;
}	t_iolist;

typedef struct s_token
{
	char			*str;
	t_quottype		type;
	t_special_c		special;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

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
	unsigned char		*status;
	int					pipefd[PIPEFD_NUM];
	enum e_cmd			cmd_type;
	t_cmdlist			*clst;
	t_iolist			*iolst;
	t_envlist			*elst;
	struct s_execdata	*next;
}	t_execdata;

t_execdata	*parse_cmd(char *command, t_envlist *envlist, int *status);

#endif
