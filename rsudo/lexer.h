#ifndef LEXER_H
#define LEXER_H
#define DEFAULT_QUOT 1
#define START_QUOT -1
#define END_QUOT 2

# include "../libft/libft.h"
# include <stdio.h>
# include <stdbool.h>

enum e_type
{
	DEFAULT,
	D_QUART,
	S_QUART
};

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
	char		*str;
	enum e_type	type;
	enum e_built_cmd	built;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

void put_list(t_token *list);

#endif