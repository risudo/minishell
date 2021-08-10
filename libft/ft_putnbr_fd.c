#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	long_n;

	long_n = n;
	if (long_n < 0)
	{
		long_n *= -1;
		write(fd, "-", 1);
	}
	if (long_n > 9)
	{
		ft_putnbr_fd(long_n / 10, fd);
		ft_putnbr_fd(long_n % 10, fd);
	}
	else
		ft_putchar_fd((int)(long_n + '0'), fd);
}
