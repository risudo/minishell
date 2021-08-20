#include "lexer.h"

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
	// i--;
	// while (prev)
	// {
	// 	printf("[%d] [%s]\ntype: %d\nspecial: %d\n\n",
	//		i, prev->str, prev->type, prev->special);
	// 	prev = prev->prev;
	// 	i--;
	// }

void	put_cmdlist(t_cmdlist *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("\t\033[32mcmdlist[%d]\033[39m: %s\n", i, list->str);
		list = list->next;
		i++;
	}
}

void	put_iolist(t_iolist *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("\t\033[32miolist[%d]\033[39m\n\t  str: %s\n", i, list->str);
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
}

void	put_execdata(t_execdata *data)
{
	int	i;

	i = 0;
	while (data)
	{
		printf("\033[41mexecdata[%d]\033[49m\n", i);
		printf("\tin_fd: %d\n", data->in_fd);
		printf("\tout_fd: %d\n\n", data->out_fd);
		put_cmdlist(data->clst);
		printf("\n");
		put_iolist(data->iolst);
		printf("\n");
		data = data->next;
		i++;
	}
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

int	main(int argc, char **argv, char **envp)
{
	t_execdata	*data;

	char		*cmd1 = "<in echo   'test hoge hoge'  \"fuga \" |   cat|cat>>file1 >> file2";
	char		*cmd2 = "echo test || cat";
	char		*cmd3 = " >>file1 echo aaa bbb >> file2";
	char		*cmd4 = " >>file1 echo aaa>> file2| cat";
	char		*cmd5 = "<<LIMITER < > file2 echo aaa | < abc cat < file3 | hoge hoge";

	// printf("cmd: [%s]\n", cmd5);
	data = parse_cmd(cmd1, envp);
	if (data)
		put_execdata(data);
	clear_execdata(data);
	(void)cmd1, (void)cmd2, (void)cmd3, (void)cmd4;
	(void)cmd5, (void)argc, (void)argv;
	if (system("leaks a.out > /dev/null"))
		system("leaks a.out");
}
