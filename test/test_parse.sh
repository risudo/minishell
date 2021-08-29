#!/bin/bash

./parse 'echo aaa|cat' > result.txt

./parse 'echo aaa |cat' >> result.txt

./parse 'echo aaa| cat' >> result.txt

./parse '<file cat' >> result.txt

./parse 'echo "aaa | bbb"' >> result.txt

./parse 'echo "aaa > bbb"' >> result.txt

./parse 'echo "aaa << bbb"' >> result.txt

# expand $?
./parse 'echo $?' >> result.txt

./parse 'echo "$?"' >> result.txt

./parse 'echo '\''$?'\' >> result.txt

./parse 'echo $?$?' >> result.txt

./parse 'echo $?aa' >> result.txt

./parse 'echo >$?aa' >> result.txt

# expand variable
./parse 'echo $USER' >> result.txt

./parse 'echo "$USER"' >> result.txt

./parse 'echo '\''"$USER"'\' >> result.txt
#echo '"$VAR"'

./parse 'echo '\'\''$USER'\'\' >> result.txt

./parse 'echo ""$USER""' >> result.txt

./parse 'echo ''"'\''"$USER"'\''"' >> result.txt
#echo "'"$USER"'"

export VAR="   ramen"

./parse 'echo "aa$VAR"' >> result.txt

./parse 'echo aa$VAR' >> result.txt

# redirect
./parse 'echo >$USER' >> result.txt

./parse 'echo >"$USER"' >> result.txt

./parse 'echo >'\''"$USER"'\' >> result.txt
#echo '"$VAR"'

./parse 'echo >'\'\''$USER'\'\' >> result.txt

./parse 'echo >""$USER""' >> result.txt

./parse 'echo >''"'\''"$USER"'\''"' >> result.txt
#echo "'"$USER"'"

./parse 'echo >"aa$VAR"' >> result.txt


# error case
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