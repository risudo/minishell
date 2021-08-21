make -C ../libft
gcc -I ../libft minipipe.c ../libft/libft.a
./a.out
