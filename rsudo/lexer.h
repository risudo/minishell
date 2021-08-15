#ifndef LEXER_H
# define LEXER_H
// # define DEFAULT 1
// # define START_QUOT -1
// # define END_QUOT 2

# include "../libft/libft.h"
# include <stdio.h>
# include <stdbool.h>

typedef enum e_quottype
{
	DEFAULT,
	D_QUOT,
	S_QUOT,
	END_S_QUOT,
	END_D_QUOT
}	t_quottype;

enum e_built_cmd
{
	ECHO,
	ECHO_N,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
};

typedef struct s_token
{
	char			*str;
	t_quottype		type;
	// enum e_built_cmd	built;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

void	parse_cmd(char *command);
void	put_list(t_token *list);
t_token	*tokenize_cmd_by_space(char *cmd);
t_token	*split_operater(t_token *list);
void	clear_tokenlist(t_token *list);

#endif