# include "rakiyama.h"

char	*ft_getenv(t_envlist *elst, char *search_key)
{
	while (elst)
	{
		if (ft_strcmp(elst->key, search_key) == 0)
			return (elst->value);
		elst = elst->next;
	}
	return (NULL);
}

t_envlist	*ft_unsetenv(t_envlist *elst, char *rm_key)
{
	t_envlist	*head;
	t_envlist	*prev;

	head = elst;
	prev = NULL;
	while (elst)
	{
		if (ft_strcmp(elst->key, rm_key) == 0)
		{
			if (prev == NULL)
				head = elst->next;
			else
				prev->next = elst->next;
			free(elst->key);
			free(elst->value);
			free(elst);
			break ;
		}
		prev = elst;
		elst = elst->next;
	}
	return (head);
}

static t_envlist	*get_elst_node(char *new_key, char *new_value)
{
	t_envlist	*new_elst;

	new_elst = ft_xcalloc(1, sizeof(*new_elst));
	new_elst->key = new_key;
	new_elst->value = new_value;
	return (new_elst);
}

void	ft_setenv(t_envlist *elst, char *new_key, char *new_value, int append)
{
	char		*add_value;

	while(1)
	{
		if (ft_strcmp(elst->key, new_key) == 0)
		{
			if (new_value)
			{
				if (append)
					add_value = ft_xstrjoin(elst->value, new_value);
				else
					add_value = ft_xstrdup(new_value);
				free(new_value);
				free(elst->value);
				elst->value = add_value;
			}
			free(new_key);
			return ;
		}
		if (elst->next == NULL)
			break ;
		elst = elst->next;
	}
	elst->next = get_elst_node(new_key, new_value);
}