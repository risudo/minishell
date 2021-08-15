#include "lexer.h"

void	clear_tokenlist(t_token *list)
{
	t_token	*next;

	while (list)
	{
		next = list->next;
		//clear list->str and list
		free(list->str);
		free(list);
		list = next;
	}
}
