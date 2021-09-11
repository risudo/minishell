make -C ../

rm -f error_result.txt result.txt minishell.txt bash.txt \
	error_bash.txt error_minishell.txt \
	minishell_status.txt bash_status.txt \


export A="   rsudo"
export SPACE="    "
export SPACE2=" aa   bb    "
export FILE1="f"
export FILE2='$FILE1'
export IN_RED="<"
export ECHO="echo -n"
export AKIYAMA=akiyama=ryojiro
echo hello > hello.txt

echo "\033[2J\033[1;1H"
while read line
do
 echo '"'$line'"'

 echo $line | ../minishell >> result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line >> minishell_status.txt

 echo $line | bash >> bash.txt 2>&1
 echo $? >> bash_status.txt
 echo $line >> bash_status.txt

 echo "\033[1;1H\033[0J"
done < ./cmdline.txt

while read line
do
 echo '"'$line'"'

 echo $line >> error_result.txt
 echo $line | ../minishell >> error_result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line >> minishell_status.txt

 echo $line >> error_bash_txt
 echo $line | bash >> error_bash.txt 2>&1
 echo $? >> bash_status.txt
 echo $line >> bash_status.txt

 echo "\033[1;1H\033[0J"
done < ./error_cmdline.txt



sed '/minishell\$ /d' result.txt > minishell.txt
sed '/minishell\$ /d' error_result.txt > error_minishell.txt

echo '--NORMAL TEST--'
diff -s minishell.txt bash.txt \
| sed "s/Files minishell.txt and bash.txt are identical/OK/g" \
| GREP_COLOR='23;32' grep -E --color "$|OK"


echo '\n--STATUS TEST--'
diff -s minishell_status.txt bash_status.txt \
| sed "s/Files minishell_status.txt and bash_status.txt are identical/OK/g" \
| GREP_COLOR='23;32' grep -E --color "$|OK"

rm hello.txt result.txt error_result.txt f

# rm minishell.txt bash.txt
# rm minishell_status.txt bash_status.txt
rm error_bash.txt error_minishell.txt

unset A SPACE SPACE2 FILE1 FILE2 AKIYAMA ECHO IN_RED
