make -C ../libft
gcc -I ../includes/ minipipe.c ../srcs/wrapper1.c ../srcs/wrapper2.c ../srcs/envlist.c ../libft/libft.a
./a.out
