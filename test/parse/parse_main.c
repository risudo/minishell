#include "../includes/minishell.h"

unsigned char	g_status = 0;

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
		data = parse_cmd(argv[1], envlist);
		*status = g_status;
		put_execdata(data);
		clear_envlist(envlist);
		xfree(status);
		clear_execdata(data);
	}
	return (0);
}
