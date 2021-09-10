make -C ../

rm -f error_result.txt result.txt minishell.txt bash.txt \
	bash_error.txt minishell_error.txt \
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

while read line
do
 echo $line | ../minishell >> result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line | bash >> bash.txt 2>&1
 echo $? >> bash_status.txt
done < ./cmdline.txt

while read line
do
 echo $line | ../minishell >> error_result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line | bash >> error_bash.txt 2>&1
 echo $? >> bash_status.txt
done < ./error_cmdline.txt



sed '/minishell\$ /d' result.txt > minishell.txt
sed '/minishell\$ /d' error_result.txt > error_minishell.txt

diff -s minishell.txt bash.txt \
| sed "s/Files minishell.txt and bash.txt are identical/<NORMAL TEST OK>/g" \
| GREP_COLOR='23;32' grep -E --color "$|<NORMAL TEST OK>"

diff -s minishell_status.txt bash_status.txt \
| sed "s/Files minishell_status.txt and bash_status.txt are identical/<STATUS TEST OK>/g" \
| GREP_COLOR='23;32' grep -E --color "$|<STATUS TEST OK>"

rm minishell.txt bash.txt
rm minishell_status.txt bash_status.txt
rm hello.txt result.txt error_result.txt
# rm error_bash.txt error_minishell.txt

unset A SPACE SPACE2 FILE1 FILE2 AKIYAMA ECHO IN_RED