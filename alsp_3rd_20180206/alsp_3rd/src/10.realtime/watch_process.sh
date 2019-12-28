#!/bin/bash
[ $# != 1 ] && echo "Usage $0 <process name>" && exit 1
TARGET_ARGV0=$1
PS_LIST="ps -o pid,priority,nice,cmd "

while :
do
    TARGET_PID=$(pgrep -fx "$TARGET_ARGV0")
    if [ -z "$TARGET_PID" ]; then
        sleep 1
        echo "$TARGET_ARGV0 is not found"
        continue
    fi
    ${PS_LIST} ${TARGET_PID}
	echo ""
	sleep 1
done
