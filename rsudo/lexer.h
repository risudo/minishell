#ifndef LEXER_H
# define LEXER_H

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

typedef struct s_token
{
	char			*str;
	t_quottype		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

void	parse_cmd(char *command);
void	put_list(t_token *list);
t_token	*tokenize_cmd_by_space(char *cmd);
t_token	*split_operater(t_token *list);
void	clear_tokenlist(t_token *list);

#endif