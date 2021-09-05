#include "minishell.h"

void	put_execdata(t_execdata *data)
{
	int	i;

	i = 0;
	while (data)
	{
		printf("execdata[%d]\n", i);
		printf("\t*status: %hhu\n", g_status);
		put_cmdlist(data->clst);
		put_iolist(data->iolst);
		data = data->next;
		i++;
	}
}
