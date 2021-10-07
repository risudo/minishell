#!/bin/bash
echo ------------ps--------------
ps | tee ps_current_process.txt | sed '/grep/d' | GREP_COLOR='0;35' egrep --color=auto '.*minishel.*|$'
echo ------------ps--------------
pid=`cat ps_current_process.txt | grep minishell | awk '{print $1}'`
echo -e "\033[35mPID->${pid}\033[m"
sleep 5
rm ps_current_process.txt
while [ 1 ]
do
lsof -p $pid
sleep 1
done
