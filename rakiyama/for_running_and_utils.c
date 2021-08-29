#include "rakiyama.h"

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

t_cmdlist	*add_cmdlist(t_cmdlist *clst, char *s)
{
	t_cmdlist	*tmp;
	t_cmdlist	*move;

	tmp = (t_cmdlist *)malloc(sizeof(t_cmdlist));
	tmp->str = s;
	tmp->next = NULL;
	if (clst == NULL)
		return(tmp);
	move = clst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (clst);
}

t_iolist	*add_iolist(t_iolist *iolst, int c, char *s, int h)
{
	t_iolist	*tmp;
	t_iolist	*move;

	tmp = (t_iolist *)malloc(sizeof(t_iolist));
	tmp->c_type = c;
	tmp->str = s;
	tmp->here_doc_fd = h;
	tmp->next = NULL;
	if (iolst == NULL)
		return(tmp);
	move = iolst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (iolst);
}

t_envlist	*add_envlist(t_envlist *elst, char *k, char *v)
{
	t_envlist	*tmp;
	t_envlist	*move;

	tmp = (t_envlist *)malloc(sizeof(t_envlist));
	tmp->key = k;
	tmp->value = v;
	tmp->next = NULL;
	if (elst == NULL)
		return(tmp);
	move = elst;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (elst);
}

t_execdata	*add_execdata(t_execdata *data, int *s, t_cmdlist *clst, t_iolist *iolst, t_envlist *elst)
{
	t_execdata	*tmp;
	t_execdata	*move;

	tmp = (t_execdata *)malloc(sizeof(t_execdata));
	tmp->cmdline = NULL;
	tmp->in_fd = STDIN_FILENO;
	tmp->out_fd = STDOUT_FILENO;
	tmp->status = s;
	tmp->clst = clst;
	tmp->iolst = iolst;
	tmp->elst = elst;
	tmp->next = NULL;
	if (data == NULL)
		return(tmp);
	move = data;
	while (move->next)
		move = move->next;
	move->next = tmp;
	return (data);
}

void	put_data(t_execdata *data)
{
	t_cmdlist	*ctmp;
	t_iolist	*iotmp;
	t_envlist	*etmp;

	while(data)
	{
		printf("--------------------------------------------\n");
		printf("status:%d\n", *(data->status));
		ctmp = data->clst;
		while (ctmp)
		{
			printf("cmdlist:%s\n", ctmp->str);
			ctmp = ctmp->next;
		}
		iotmp = data->iolst;
		while (iotmp)
		{
			printf("iolist:%s\n", iotmp->str);
			iotmp = iotmp->next;
		}
		etmp = data->elst;
		while (etmp)
		{
			printf("env key:%s\n", etmp->key);
			printf("env value:%s\n", etmp->value);
			etmp = etmp->next;
		}
		data = data->next;
	}
}

void	free_data(t_execdata *data, int status_free, int elst_free)
{
	void		*tmp;
	t_envlist	*elst_tmp;

	if (status_free)
		free(data->status);
	while (elst_free && data->elst)
	{
		elst_tmp = data->elst;
		data->elst = data->elst->next;
		free(elst_tmp->key);
		free(elst_tmp->value);
		free(elst_tmp);
	}
	while(data)
	{
		while (data->clst)
		{
			tmp = data->clst;
			data->clst = data->clst->next;
			free(tmp);
		}
		while (data->iolst)
		{
			tmp = data->iolst;
			data->iolst = data->iolst->next;
			free(tmp);
		}
		tmp = data;
		data = data->next;
		free(tmp);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_cmdlist	*clst;
	t_iolist	*iolst;
	t_envlist	*elst;
	t_execdata	*data;
	int			*status;
	t_envlist	*etmp;
	int			exit_status;


	data = NULL;
	elst = NULL;
	elst = add_envlist(elst, ft_strdup("HOME"), ft_strdup("/Users/ryojiroakiyama"));
	elst = add_envlist(elst, ft_strdup("PATH"), ft_strdup("/Users/ryojiroakiyama/.pyenv/shims:/Users/ryojiroakiyama/.pyenv/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands"));
	elst = add_envlist(elst, ft_strdup("EEE"), ft_strdup("aaaaaaai"));
	elst = add_envlist(elst, ft_strdup("PWD"), ft_strdup("/Users/ryojiroakiyama/Desktop/icloud/42_cursus/minishell/minishell/rakiyama"));
	elst = add_envlist(elst, ft_strdup("RYOJIRO"), ft_strdup("ryojiro"));
	elst = add_envlist(elst, ft_strdup("AKIYAMA"), ft_strdup("akiyama"));
	status = (int *)malloc(sizeof(int));
	*status = 0;
	clst = NULL;
	clst = add_cmdlist(clst, "echo");
	clst = add_cmdlist(clst, "akiyama");
	iolst = NULL;
	iolst = add_iolist(iolst, IN_HERE_DOC, "<<", -1);
	iolst = add_iolist(iolst, ELSE, "limit", -1);
	iolst = add_iolist(iolst, OUT_REDIRECT, ">", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "cat");
	clst = add_cmdlist(clst, "-n");	
	iolst = NULL;
	iolst = add_iolist(iolst, IN_REDIRECT, "<", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	iolst = add_iolist(iolst, OUT_HERE_DOC, ">>", -1);
	iolst = add_iolist(iolst, ELSE, "outfile", -1);
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "env");
	iolst = NULL;
	iolst = add_iolist(iolst, OUT_REDIRECT, ">", -1);
	iolst = add_iolist(iolst, ELSE, "envoutfile", -1);
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "export");
	clst = add_cmdlist(clst, "EEE+=uuuuuuuui");	
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 0, 0);

	data = NULL;
	clst = NULL;
	clst = add_cmdlist(clst, "exit");
	clst = add_cmdlist(clst, "5829039");
	iolst = NULL;
	data = add_execdata(data, status, clst, iolst, elst);
	execute_start(data);
	exit_status = *(data->status);
	free_data(data, 1, 1);

//	put_data(data);

	if (system("leaks a.out > /dev/null"))
		system("leaks a.out");
	return (exit_status);
}