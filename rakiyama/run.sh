make -C ../libft
gcc -lreadline -lhistory -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -I ../includes/ *.c ../srcs/wrapper1.c ../srcs/wrapper2.c ../libft/libft.a
#./a.out
