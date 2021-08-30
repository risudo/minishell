make -C ../libft
gcc -lreadline -lhistory -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -I ../includes/ ../srcs/*.c for_running_and_utils.c ../libft/libft.a
./a.out
rm a.out
make fclean -C ../libft
