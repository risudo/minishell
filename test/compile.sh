# gcc -Wall -Wextra -Werror -fsanitize=address \
# gcc -Wall -Wextra -Werror -fsanitize=undefined \
gcc -Wall -Wextra -Werror -o parse\
	parse_main.c \
	parse_cmd.c \
	parse_tokenlist.c \
	tokenize*.c \
	clear_list.c \
	execdata.c \
	execdata_utils.c \
	envlist.c \
	test_parse.c \
	wrapper1.c \
	wrapper2.c \
	check_syntax.c \
	expansion_cmdlist.c \
	expansion_iolist.c \
	expansion_utils1.c \
	expansion_utils2.c \
	../libft/libft.a