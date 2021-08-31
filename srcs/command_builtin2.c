#include "minishell.h"

/*
** Execute built-in command.
** - export
*/

static ssize_t	check_name_rule(char *src_str)
{
	ssize_t	i;

	i = 0;
	while (src_str[i])
	{
		if (0 < i && \
			((src_str[i] == '+' && src_str[i + 1] == '=') || \
			src_str[i] == '='))
			break ;
		if (!(src_str[i] == '_' || \
			ft_isalnum(src_str[i])) || \
			ft_isdigit(src_str[0]))
			return (-1);
		i++;
	}
	return (i);
}

static void	to_setenv(t_envlist *head, char *src_str, size_t i)
{
	char	*key;
	char	*value;
	int		mode;

	if (src_str[i] == '+')
	{
		key = ft_xsubstr(src_str, 0, i);
		value = ft_xsubstr(src_str, i + 2, ft_strlen(src_str) - i - 2);
		mode = 1;
	}
	else if (src_str[i] == '=')
	{
		key = ft_xsubstr(src_str, 0, i);
		value = ft_xsubstr(src_str, i + 1, ft_strlen(src_str) - i - 1);
		mode = 0;
	}
	else
	{
		key = ft_xstrdup(src_str);
		value = NULL;
		mode = 0;
	}
	ft_setenv(head, key, value, mode);
}

static void	put_env_asciiorder(t_envlist *head, t_envlist *min_node)
{
	t_envlist	*put_node;
	t_envlist	*move;

	put_node = NULL;
	move = head;
	while (move)
	{
		if (min_node == NULL || ft_strcmp(min_node->key, move->key) < 0)
		{
			if (put_node == NULL)
				put_node = move;
			else if (ft_strcmp(move->key, put_node->key) < 0)
				put_node = move;
		}
		move = move->next;
	}
	if (put_node)
	{
		printf("declare -x %s=\"%s\"\n", put_node->key, put_node->value);
		put_env_asciiorder(head, put_node);
	}
}

void	builtin_export(t_execdata *data)
{
	size_t	arg_i;
	ssize_t	split_point;

	*(data->status) = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] == NULL)
		put_env_asciiorder(data->elst, NULL);
	else
	{
		while (data->cmdline[arg_i])
		{
			split_point = check_name_rule(data->cmdline[arg_i]);
			if (split_point != -1)
				to_setenv(data->elst, data->cmdline[arg_i], split_point);
			else
			{
				ft_putstr_fd("minishell: export: ", STDERR_FILENO);
				ft_putstr_fd(data->cmdline[arg_i], STDERR_FILENO);
				ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
				*(data->status) = 1;
			}
			arg_i++;
		}
	}
}
