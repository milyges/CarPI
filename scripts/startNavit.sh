#!/bin/sh

while true
do
	WID="$(xdotool search --onlyvisible --name navit 2>/dev/null)"
	if [ -n "${WID}" ]
	then
		echo "${WID}"
		break
	fi
	sleep 0.2
done &

exec 1> /tmp/navit.log
exec 2>&1
exec navit
