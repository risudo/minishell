#include "../includes/parse.h"

void	clear_envlist(t_envlist *envlist)
{
	t_envlist	*next;

	while (envlist)
	{
		next = envlist->next;
		xfree(envlist->key);
		xfree(envlist->value);
		xfree(envlist);
		envlist = next;
	}
}

void	clear_tokenlist(t_token *list)
{
	t_token	*next;

	while (list)
	{
		next = list->next;
		xfree(list->str);
		xfree(list);
		list = next;
	}
}

void	clear_cmdlist(t_cmdlist *list)
{
	t_cmdlist	*next;

	while (list)
	{
		next = list->next;
		xfree(list->str);
		xfree(list);
		list = next;
	}
}

void	clear_iolist(t_iolist *list)
{
	t_iolist	*next;

	while (list)
	{
		next = list->next;
		xfree(list->str);
		xfree(list);
		list = next;
	}
}

void	clear_execdata(t_execdata *data)
{
	t_execdata	*next;

	clear_envlist(data->elst);
	xfree(data->status);
	while (data)
	{
		next = data->next;
		clear_cmdlist(data->clst);
		clear_iolist(data->iolst);
		xfree(data);
		data = next;
	}
}
