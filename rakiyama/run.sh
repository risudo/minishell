make -C ../libft
gcc -I ../includes/ minipipe.c ../srcs/wrapper2.c ../libft/libft.a
./a.out
