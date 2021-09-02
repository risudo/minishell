#!/bin/bash

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
../srcs/tokenize1.c \
../srcs/tokenize2.c \
../srcs/tokenize_utils.c \
../srcs/wrapper1.c \
../srcs/wrapper2.c \
../srcs/wrapper3.c \
../libft/libft.a


./parse 'echo aaa|cat' > result.txt

./parse '   echo aaa|cat' >> result.txt

./parse 'echo aaa|cat   ' >> result.txt

./parse 'echo aaa |cat' >> result.txt

./parse 'echo aaa| cat' >> result.txt

./parse '<file cat' >> result.txt

./parse 'echo "aaa | bbb"' >> result.txt

./parse 'echo "aaa > bbb"' >> result.txt

./parse 'echo "aaa << bbb"' >> result.txt

./parse 'echo $' >> result.txt

./parse 'echo$' >> result.txt

./parse 'echo $ | cat' >> result.txt


# expand $?
./parse 'echo $?' >> result.txt

./parse 'echo "$?"' >> result.txt

./parse 'echo '\''$?'\' >> result.txt

./parse 'echo $?$?' >> result.txt

./parse 'echo $?aa' >> result.txt

./parse 'echo >$?aa' >> result.txt

### expand variable
./parse 'echo $USER' >> result.txt

./parse 'echo "$USER"' >> result.txt

./parse 'echo '\''"$USER"'\' >> result.txt
#echo '"$VAR"'

./parse 'echo '\'\''$USER'\'\' >> result.txt

./parse 'echo ""$USER""' >> result.txt

./parse 'echo ''"'\''"$USER"'\''"' >> result.txt

./parse 'echo $USER?' >> result.txt

./parse 'echo $USER_' >> result.txt

./parse 'echo $USER!' >> result.txt
#echo "'"$USER"'"

export VAR="   ramen"

./parse 'echo "aa$VAR"' >> result.txt

./parse 'echo aa$VAR' >> result.txt

### redirect
./parse 'echo >$USER' >> result.txt

./parse 'echo >"$USER"' >> result.txt

./parse 'echo >'\''"$USER"'\' >> result.txt
#echo '"$VAR"'

./parse 'echo >'\'\''$USER'\'\' >> result.txt

./parse 'echo >""$USER""' >> result.txt

./parse 'echo >''"'\''"$USER"'\''"' >> result.txt
#echo "'"$USER"'"

./parse 'echo >"aa$VAR"' >> result.txt


### error case
./parse 'echo >aa$VAR' >> result.txt 2>&1

./parse '| echo aaa' >> result.txt 2>&1

./parse 'echo aaa |' >> result.txt 2>&1

./parse 'echo aaa >' >> result.txt 2>&1

./parse 'echo aaa <' >> result.txt 2>&1

./parse 'echo aaa >>' >> result.txt 2>&1

./parse 'echo aaa <<' >> result.txt 2>&1

./parse 'echo aaa >>>' >> result.txt 2>&1

./parse 'echo "aa' >> result.txt 2>&1

./parse 'echo '\''aa' >> result.txt 2>&1


diff -s result.txt answer.txt \
| sed "s/Files result.txt and answer.txt are identical/<TEST OK>/g" \
| GREP_COLOR='23;32' grep -E --color "$|<TEST OK>"

# rm parse