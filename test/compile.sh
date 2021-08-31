make -C ../libft
# gcc -Wall -Wextra -Werror -fsanitize=address \
# gcc -Wall -Wextra -Werror -fsanitize=undefined \
gcc -Wall -Wextra -Werror -lreadline -L /usr/local/opt/readline -I /usr/local/opt/readline -I ../includes/ -o parse \
parse_main.c \
test_parse.c \
test_parse2.c \
../srcs/clear_list.c \
../srcs/command_builtin1.c \
../srcs/command_builtin2.c \
../srcs/command_builtin3.c \
../srcs/command_nonbuiltin.c \
../srcs/env_functions.c \
../srcs/envlist.c \
../srcs/execdata.c \
../srcs/execdata_utils.c \
../srcs/execution_setdata.c \
../srcs/execution_start.c \
../srcs/execution_utils.c \
../srcs/execution_utils_libft.c \
../srcs/execution_utils_list.c \
../srcs/expansion_cmdlist.c \
../srcs/expansion_iolist.c \
../srcs/expansion_utils1.c \
../srcs/expansion_utils2.c \
../srcs/parse_cmd.c \
../srcs/parse_tokenlist.c \
../srcs/signal_handler.c \
../srcs/tokenize1.c \
../srcs/tokenize2.c \
../srcs/tokenize_utils.c \
../srcs/wrapper1.c \
../srcs/wrapper2.c \
../srcs/wrapper3.c \
../libft/libft.a
