#include "../includes/parse.h"

void	set_key_and_value(char *env, t_envlist *new)
{
	char	*key;
	char	*value;
	char	*p_equal;
	int		key_len;

	p_equal = ft_strchr(env, '=');
	key_len = p_equal - env;
	key = ft_xsubstr(env, 0, key_len);
	value = ft_xsubstr(env, key_len + 1, ft_strlen(env) - key_len - 1);
	new->key = key;
	new->value = value;
}

t_envlist	*new_envlist(t_envlist *cur, char *env)
{
	t_envlist	*new;

	new = ft_xcalloc(1, sizeof(*new));
	set_key_and_value(env, new);
	cur->next = new;
	return (new);
}

t_envlist	*create_envlist(char **envp)
{
	t_envlist	head;
	t_envlist	*cur;
	int			i;

	i = 0;
	cur = &head;
	while (envp[i])
	{
		cur = new_envlist(cur, envp[i]);
		i++;
	}
	return (head.next);
}