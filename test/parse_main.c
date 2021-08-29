#include "../includes/minishell.h"

void	put_execdata(t_execdata *data)
{
	int	i;

	i = 0;
	while (data)
	{
		printf("\033[41mexecdata[%d]\033[49m\n", i);
		printf("\t*status: %hhu\n", *(data->status));
		printf("\tin_fd: %d\n", data->in_fd);
		printf("\tout_fd: %d\n\n", data->out_fd);
		put_cmdlist(data->clst);
		put_iolist(data->iolst);
		data = data->next;
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_execdata		*data;
	t_envlist		*envlist;
	unsigned char	*status;

	(void)argc;
	envlist = create_envlist(envp);
	status = (unsigned char *)ft_xcalloc(1, sizeof(int));
	if (argv[1])
	{
		printf("command: [%s]\n", argv[1]);
		data = parse_cmd(argv[1], envlist, status);
		put_execdata(data);
		clear_envlist(envlist);
		xfree(status);
		clear_execdata(data);
	}
	return (0);
}
