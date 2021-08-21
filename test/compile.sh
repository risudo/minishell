gcc -Wall -Wextra -Werror \
parse_cmd*.c clear_list.c execdata.c execdata_utils.c envlist.c test_parse.c wrapper1.c ../libft/libft.a
./a.out
rm a.out