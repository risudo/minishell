make -C ../../libft
# gcc -Wall -Wextra -Werror -fsanitize=address \
# gcc -Wall -Wextra -Werror -fsanitize=undefined \
READLINE_DIR=`brew --prefix readline`

gcc -Wall -Wextra -Werror -I${READLINE_DIR}/include -I ../../includes/ -L${READLINE_DIR} -lreadline -o parse \
parse_main.c \
test_parse.c \
test_parse2.c \
../../srcs/clear_list.c \
../../srcs/command_builtin1.c \
../../srcs/command_builtin2.c \
../../srcs/command_builtin3.c \
../../srcs/command_nonbuiltin.c \
../../srcs/env_functions.c \
../../srcs/envlist.c \
../../srcs/execdata.c \
../../srcs/delimit_fd.c \
../../srcs/setdata_cmdline_redirection.c \
../../srcs/setdata_heredoc_cmdtype.c \
../../srcs/execdata_utils.c \
../../srcs/execution_start.c \
../../srcs/execution_utils.c \
../../srcs/execution_utils_libft.c \
../../srcs/execution_utils_list.c \
../../srcs/expansion_cmdlist.c \
../../srcs/expansion_iolist.c \
../../srcs/expansion_io_utils.c \
../../srcs/expansion_utils1.c \
../../srcs/expansion_utils2.c \
../../srcs/parse_cmd.c \
../../srcs/parse_tokenlist.c \
../../srcs/tokenize1.c \
../../srcs/tokenize2.c \
../../srcs/tokenize_utils.c \
../../srcs/wrapper1.c \
../../srcs/wrapper2.c \
../../srcs/wrapper3.c \
../../srcs/wrapper4.c \
../../srcs/signal_handler.c \
../../libft/libft.a
