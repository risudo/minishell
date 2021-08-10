#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ans;
	size_t	msize;

	msize = count * size;
	ans = malloc(msize);
	if (!ans)
		return (NULL);
	if (!msize)
		msize = 1;
	ft_bzero(ans, msize);
	return (ans);
}
