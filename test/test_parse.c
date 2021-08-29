#include "../includes/parse.h"

void	end(void)__attribute__((destructor));

void	end(void)
{
	if (system("leaks parse > /dev/null"))
		system("leaks parse");
}

void	put_tokenlist(t_token *list)
{
	int		i;
	t_token	*prev;

	i = 0;
	while (list)
	{
		printf("[%d] [%s]\ntype: %d\nspecial: %d\n\n",
			i, list->str, list->type, list->special);
		prev = list;
		list = list->next;
		i++;
	}
}

void	put_cmdlist(t_cmdlist *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("\t\033[32mcmdlist[%d]\033[39m\n\t  str: \"%s\"\n", i, list->str);
		printf("\t  quot: %s\n", list->quot);
		list = list->next;
		i++;
	}
	printf("\n");
}

void	put_iolist(t_iolist *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("\t\033[35miolist[%d]\033[39m\n\t  str: \"%s\"\n", i, list->str);
		printf("\t  quot: %s\n", list->quot);
		if (list->c_type == IN_REDIRECT)
			printf("\t  c_type: IN_REDIRECT\n");
		if (list->c_type == OUT_REDIRECT)
			printf("\t  c_type: OUT_REDIRECT\n");
		if (list->c_type == IN_HERE_DOC)
			printf("\t  c_type: IN_HERE_DOC\n");
		if (list->c_type == OUT_HERE_DOC)
			printf("\t  c_type: OUT_HERE_DOC\n");
		if (list->c_type == PIPE)
			printf("\t  c_type: PIPE\n");
		if (list->c_type == ELSE)
			printf("\t  c_type: ELSE\n");
		printf("\t  here_doc_fd: %d\n\n", list->here_doc_fd);
		list = list->next;
		i++;
	}
	printf("\n");
}

void	put_envlist(t_envlist *envlist)
{
	int	i;

	i = 0;
	printf("\033[41menvlist\033[49m\n");
	while (envlist)
	{
		printf("\t[%d] kew: %s\tvalue: %s\n", i, envlist->key, envlist->value);
		envlist = envlist->next;
		i++;
	}
}
