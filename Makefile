NAME = minishell

SRCS_DIR = ./srcs/

SRCS_NAME = main.c \
			clear_list.c \
			command_builtin1.c \
			command_builtin2.c \
			command_builtin3.c \
			command_nonbuiltin.c \
			env_functions.c \
			envlist.c \
			execdata.c \
			execdata_utils.c \
			delimit_fd.c \
			setdata_cmdline_redirection.c \
			setdata_heredoc_cmdtype.c \
			execution_start.c \
			execution_utils.c \
			execution_utils_libft.c \
			execution_utils_list.c \
			expansion_cmdlist.c \
			expansion_iolist.c \
			expansion_io_utils.c \
			expansion_utils1.c \
			expansion_utils2.c \
			minishell_loop.c \
			parse_cmd.c \
			parse_tokenlist.c \
			tokenize1.c \
			tokenize2.c \
			tokenize_utils.c \
			wrapper1.c \
			wrapper2.c \
			wrapper3.c \
			wrapper4.c

SRCS = ${addprefix ${SRCS_DIR}, ${SRCS_NAME}}

HEADER_DIR = ./includes

LIBFT_DIR = ./libft

READLINE_DIR = $(shell brew --prefix readline)

OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I${READLINE_DIR}/include -I${HEADER_DIR} -I${LIBFT_DIR}

RM = rm -rf

all: ${NAME}

${NAME}: ${OBJS} ${HEADER_DIR}
	@${MAKE} -C ${LIBFT_DIR}
	${CC} ${C_FLAGS} -o ${NAME} ${OBJS} -L${READLINE_DIR}/lib -lreadline -lhistory -L${LIBFT_DIR} -lft

val:
	valgrind --leak-check=full -s --show-leak-kinds=all ./${NAME}

clean:
	${MAKE} -C ${LIBFT_DIR} clean
	${RM} ${OBJS}

fclean: clean
	${MAKE} -C ${LIBFT_DIR} fclean
	${RM} ${NAME}

re: fclean all

.PHONY: all bonus val val_bonus clean fclean re
