/*

cmd == "echo $USER | cat |cat >outfile"
list->cmd = "echo"
list->next->cmd = "$USER"
list->next->next->cmd = "|""
list->next->next->next->cmd = "cat"
list->next->next->next->next->cmd = "|"
list->next->next->next->next->next->cmd = "cat"
list->next->next->next->next->next->next->cmd = ">"
list->next->next->next->next->next->next->next->cmd = "outfile"

*/
#include "lexer.h"

void	put_list(t_token *list)
{
	int	i = 0;

	while (list)
	{
		printf("[%d] [%s]\n", i, list->str);
		list = list->next;
		i++;
	}
}