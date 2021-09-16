#!/bin/bash

./compile.sh

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
echo 'VAR="   ramen"' >> result.txt

./parse 'echo "aa$VAR"' >> result.txt

./parse 'echo aa$VAR' >> result.txt

export VAR="    "
echo 'VAR="    "' >> result.txt

./parse 'echo $VAR' >> result.txt

./parse 'echo aa$VAR' >> result.txt

./parse 'echo "$VAR"' >> result.txt

./parse 'echo "$VAR"a' >> result.txt

export file=f
export FILE='$file'
echo 'file=f' >> result.txt
echo 'FILE='\''$file'\' >> result.txt

./parse 'echo $FILE' >> result.txt

./parse 'echo $FILE$file' >> result.txt

./parse 'echo $#$HOME' >> result.txt

### redirect
./parse 'echo >$USER' >> result.txt

./parse 'echo aa 10> file' >> result.txt

./parse 'echo aa 10 > file' >> result.txt

./parse 'echo aa 10>file' >> result.txt

./parse 'echo aa "10"> file' >> result.txt

./parse 'echo aa "1"0> file' >> result.txt

### space in environment variable
export VAR="  aa  bb  "
echo 'VAR="  aa  bb  "' >> result.txt

./parse 'echo $VAR' >> result.txt

./parse 'echo XX$VAR' >> result.txt

### error case
./parse 'echo >aa$VAR' >> result.txt 2>&1

./parse '| echo aaa' >> result.txt 2>&1

./parse 'echo aaa |' >> result.txt 2>&1

./parse 'echo aaa >' >> result.txt 2>&1

./parse 'echo aaa <' >> result.txt 2>&1

./parse 'echo aaa >>' >> result.txt 2>&1

./parse 'echo aaa <<' >> result.txt 2>&1

./parse 'echo aaa >>>' >> result.txt 2>&1

./parse 'echo aaa >>> a' >> result.txt 2>&1

./parse 'echo "aa' >> result.txt 2>&1

./parse 'echo '\''aa' >> result.txt 2>&1


diff -s result.txt answer.txt \
| sed "s/Files result.txt and answer.txt are identical/<TEST OK>/g" \
| GREP_COLOR='23;32' grep -E --color "$|<TEST OK>"

# rm parse