#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

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

typedef struct s_iolist //リダイレクトなど入出力に関わるリスト
{
	t_special_c		c_type; //初期値-1
	char			*str;
	int 			here_doc_fd; //初期値-1,後僕が使います
	struct s_iolist	*next;
	struct s_iolist	*prev;
}	t_iolist;

typedef struct s_token
{
	char			*str;
	t_quottype		type;
	t_special_c		special;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmdlist //リダイレクトなど以外の文字列が入るリスト
{
	char				*str;
	struct s_cmdlist	*next;
	struct s_cmdlist	*prev;
}	t_cmdlist;

typedef struct s_envlist
{
	char				*key;
	char				*value;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_execdata
{
	char				**cmdline;	 //初期値NULL、後僕が使います
	int					in_fd;		 //初期値STDIN_FILENO,後僕が使います
	int					out_fd;		 //初期値STDOUT_FILENO,後僕が使います
	int					*status;	 //どっかでint型で用意した変数(=0)のアドレスを全てのexecdataのstatusに持たせる->どのexecdataでも共有できる(※親プロセスでなら)
	//	int pipefd[PIPEFD_NUM]; //初期値指定なし、後僕が使います
	//	enum e_cmd cmd_type;
	t_cmdlist			*clst; //cmdlistが発生すれば先頭アドレスを入れる、なければNULL
	t_iolist			*iolst; //iolistが発生すれば先頭アドレスを入れる、なければNULL
	t_envlist			*elst; //全てのexecdataにも毎回同じenvlistの先頭アドレスを渡す、どのexecdataでも共有できる(※親プロセスなら)
	struct s_execdata	*next;
}	t_execdata;

#endif