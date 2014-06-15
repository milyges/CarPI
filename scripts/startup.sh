#!/bin/sh

cd /home/radio

xsetroot -solid "#000000"
xset s off
xset -dpms
xset s noblank

cd CarPI

sudo stty -F /dev/ttyUSB2 9600 -brkint -icrnl -imaxbel -opost -isig -icanon -iexten -echo

while true
do
	sudo nice -n -5 ./CarPI > /tmp/carpi.log 2>&1
done
