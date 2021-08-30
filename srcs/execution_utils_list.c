#include "execute.h"

static size_t	envlist_size(t_envlist *head)
{
	t_envlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

static size_t	cmdlist_size(t_cmdlist *head)
{
	t_cmdlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

/*
convert t_envlist type list -> char type 2d array
*/
char	**convert_envlist_2dchar(t_envlist *elst)
{
	char		**array;
	t_envlist	*move;
	size_t		cnt;
	char		*tmp;

	cnt = envlist_size(elst);
	array = (char **)ft_xcalloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = elst;
	while (move)
	{
		array[cnt] = ft_xstrjoin(move->key, "=");
		tmp = array[cnt];
		array[cnt] = ft_xstrjoin(array[cnt], move->value);
		free(tmp);
		cnt++;
		move = move->next;
	}
	return (array);
}

/*
convert t_cmdlist type list -> char type 2d array
*/
char	**convert_cmdlist_2dchar(t_cmdlist *clst)
{
	char		**array;
	t_cmdlist	*move;
	size_t		cnt;

	cnt = cmdlist_size(clst);
	array = (char **)ft_xcalloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = clst;
	while (move)
	{
		array[cnt] = ft_xstrjoin(move->str, NULL);
		cnt++;
		move = move->next;
	}
	return (array);
}
