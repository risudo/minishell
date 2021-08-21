#include "../includes/parse.h"

void	*ft_xcalloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

char	*ft_xsubstr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	ret = ft_substr(s, start, len);
	if (ret == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

char	*ft_xstrdup(const char *src)
{
	char	*ret;

	ret = ft_strdup(src);
	if (ret == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void	xfree(void *ptr)
{
	free(ptr);
	ptr = NULL;
}
