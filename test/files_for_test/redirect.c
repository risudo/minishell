#include <unistd.h>

int	main(void)
{
	write(1, "fd = 1\n", 7);
	write(2, "fd = 2\n", 7);
	write(3, "fd = 3\n", 7);
	write(42, "fd = 42\n", 8);
	write(255, "fd = 255\n", 9);
	write(256, "fd = 256\n", 9);
	write(1, "fd = 1\n", 7);
	return (0);
}
