#include "execute.h"

/*
** all command functions
*/

//echo
void	builtin_echo(t_execdata *data)
{
	int		option;
	size_t	arg_i;

	option = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] && \
		ft_strcmp(data->cmdline[arg_i], "-n") == 0 \
		&& arg_i++)
		option++;
	while (data->cmdline[arg_i])
	{
		printf("%s", data->cmdline[arg_i]);
		if (data->cmdline[arg_i + 1] != NULL)
			printf(" ");
		arg_i++;
	}
	if (option == 0)
		printf("\n");
	*(data->status) = 0;
}

//cd
void	builtin_cd(t_execdata *data)
{
	ft_setenv(data->elst, ft_xstrdup("OLDPWD"), getcwd(NULL, 0), 0);
	if (data->cmdline[1] && \
		data->cmdline[1][0] && \
		chdir(data->cmdline[1]) == -1)
	{
		perror("cd");
		*(data->status) = 1;
		return ;
	}
	if (ft_getenv(data->elst, "PWD"))
		ft_setenv(data->elst, ft_xstrdup("PWD"), getcwd(NULL, 0), 0);
	*(data->status) = 0;
}

//pwd
//if no authority to directory what happen
void	builtin_pwd(t_execdata *data)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		perror("pwd");
		*(data->status) = 1;
	}
	else
	{
		printf("%s\n", pathname);
		*(data->status) = 0;
	}
	free(pathname);
}
