#include "minishell.h"

void	put_2d_array(char **a)
{
	if (a == NULL)
	{
		fprintf(stderr, "array is NULL");
		return ;
	}
	while (*a != NULL)
	{
		fprintf(stderr, "%s\n", *a);
		a++;
	}
}

void	put_data(t_execdata *data)
{
	int			cnt_data;
	t_cmdlist	*ctmp;
	t_iolist	*iotmp;
	t_envlist	*etmp;

	cnt_data = 0;
	while(data)
	{
		fprintf(stdout, "-------------------data[%d]-------------------------\n", cnt_data);
		fprintf(stdout, "status:[%d]\n", *(data->status));
		ctmp = data->clst;
		fprintf(stdout, "cmdlist: ");
		while (ctmp)
		{
			fprintf(stdout, "[%s] ", ctmp->str);
			ctmp = ctmp->next;
		}
		fprintf(stdout, "\n");
		fprintf(stdout, "iolist: ");
		iotmp = data->iolst;
		while (iotmp)
		{
			fprintf(stdout, "[%s] ", iotmp->str);
			iotmp = iotmp->next;
		}
		fprintf(stdout, "\n");
		fprintf(stdout, "envlist\n");
		etmp = data->elst;
		while (etmp)
		{
			fprintf(stdout, "[%s]=[%s]\n", etmp->key, etmp->value);
			etmp = etmp->next;
		}
		cnt_data++;
		data = data->next;
	}
	fprintf(stdout, "-------------------------end--------------------\n");
}
