PROMPT='minishell\$ '

rm -f error_result.txt result.txt minishell.txt bash.txt \
	error_bash.txt error_minishell.txt \
	minishell_status.txt bash_status.txt \

if [ $1 = clean ] ; then
	echo 'remove output file'
	exit
elif [ $1 = fclean ] ; then
	make -C ../ fclean
	exit
else
	make -C ../
fi


export A="   rsudo"
export SPACE="    "
export SPACE2=" aa   bb    "
export FILE1="f"
export FILE2='$FILE1'
export IN_RED="<"
export ECHO="echo"
export AKIYAMA=akiyama=ryojiro
export PIPE="|"
echo hello > hello.txt

echo "\033[2J\033[1;1H"
echo "testing."
while read line
do

 echo $line | ../minishell >> result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line >> minishell_status.txt

 echo $line | bash >> bash.txt 2>&1
 echo $? >> bash_status.txt
 echo $line >> bash_status.txt

done < ./cmdline.txt
 echo "\033[1;1H\033[0J"
 echo "testing.."
while read line
do

 echo $line >> error_result.txt
 echo $line | ../minishell >> error_result.txt 2>&1
 echo $? >> minishell_status.txt
 echo $line >> minishell_status.txt

 echo $line >> error_bash.txt
 echo $line | bash >> error_bash.txt 2>&1
 echo $? >> bash_status.txt
 echo $line >> bash_status.txt

done < ./error_cmdline.txt
 echo "\033[1;1H\033[0J"


sed "/${PROMPT}/d" result.txt > minishell.txt
sed "/${PROMPT}/d" error_result.txt > error_minishell.txt

echo '--NORMAL TEST--'
diff -s minishell.txt bash.txt \
| sed "s/Files minishell.txt and bash.txt are identical/OK/g" \
| GREP_COLOR='23;32' grep -E --color "$|OK"


echo '\n--STATUS TEST--'
diff -s minishell_status.txt bash_status.txt \
| sed "s/Files minishell_status.txt and bash_status.txt are identical/OK/g" \
| GREP_COLOR='23;32' grep -E --color "$|OK"

rm hello.txt result.txt error_result.txt f outfile outfile1 outfile2

NORMAL=0
STATUS=0
ERROR=0

while getopts nse OPT
do
  case $OPT in
     n) NORMAL=1 ;;
     s) STATUS=1 ;;
     e) ERROR=1 ;;
  esac
done

if [ ${NORMAL} -eq 1 -o ${STATUS} -eq 1 -o ${ERROR} -eq 1 ] ; then
	MSG='\nLeave\n'
fi

if [ ${NORMAL} -eq 0 ]; then
	rm minishell.txt bash.txt
else
	MSG+='- normal output\n'
fi

if [ ${STATUS} -eq 0 ]; then
	rm minishell_status.txt bash_status.txt
else
	MSG+='- status output\n'
fi

if [ ${ERROR} -eq 0 ]; then
	rm error_bash.txt error_minishell.txt
else
	MSG+='- error output\n'
fi

echo $MSG

unset A SPACE SPACE2 FILE1 FILE2 AKIYAMA ECHO IN_RED PIPE
