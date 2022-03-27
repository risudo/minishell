NAME = minishell

SRCS_DIR = ./srcs/

TEST_DIR = ./test/files_for_test/

HEADER_DIR = ./includes

LIBFT_DIR = ./libft

READLINE_DIR = $(shell brew --prefix readline)

ifdef TEST_MODE
MAIN = ${TEST_DIR}test_main.c
else
MAIN = ${SRCS_DIR}main.c
endif

SRCS_NAME = clear_list.c \
			execution_echo.c \
			execution_cd.c \
			execution_pwd.c \
			execution_export.c \
			execution_unset.c \
			execution_env.c \
			execution_exit.c \
			execution_nonbuilin.c \
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
			parse_cmd.c \
			parse_tokenlist.c \
			tokenize1.c \
			tokenize2.c \
			tokenize_utils.c \
			xclose.c \
			xfork.c \
			xft_calloc.c \
			xft_itoa.c \
			xft_split.c \
			xft_strdup.c \
			xft_strjoin.c \
			xft_substr.c \
			xsignal.c \
			xwaitpid.c \
			wrapper3.c \
			wrapper4.c \
			signal_handler.c

SRCS = ${addprefix ${SRCS_DIR}, ${SRCS_NAME}}

OBJS = ${MAIN:.c=.o}

OBJS += ${SRCS:.c=.o}

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I${READLINE_DIR}/include -I${HEADER_DIR} -I${LIBFT_DIR}

RM = rm -rf

all: ${NAME}

${NAME}: ${OBJS} ${HEADER_DIR}
	@${MAKE} -C ${LIBFT_DIR}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} -L${READLINE_DIR}/lib -lreadline -lhistory -L${LIBFT_DIR} -lft

clean:
	${MAKE} -C ${LIBFT_DIR} clean
	${RM} ${OBJS}

fclean: clean
	${MAKE} -C ${LIBFT_DIR} fclean
	${RM} ${NAME}

re: fclean all

val:
	valgrind --leak-check=full -s --show-leak-kinds=all ./${NAME}

test:
	${MAKE} TEST_MODE=1

tclean:
	${MAKE} fclean TEST_MODE=1

check:
	cd ./test && ./test.sh

.PHONY: all clean fclean re val test tclean