#!/bin/sh

#-----------------------------------------------------------------------------#
#  Test gpio
#                                               Li Guang 2018.1.2
#-----------------------------------------------------------------------------#

echo "[`basename $0`]: into led and do test............"
echo "Please pay attention to the LED!"
echo "================================================="
sleep 2
gpio -led 1 on
sleep 2
gpio -led 1 off
gpio -led 2 on
sleep 2
gpio -led 2 off
gpio -led 3 on
sleep 2
gpio -led 3 off
gpio -led 4 on
sleep 2
gpio -led 4 off

echo "Please pay attention to the DO_LED!"
echo "================================================="
sleep 2
gpio -do 1 on
sleep 2
gpio -do 1 off
gpio -do 2 on
sleep 2
gpio -do 2 off

echo "Please listen to the buzzer!"
echo "================================================="
sleep 2
gpio -beep on
sleep 1
gpio -beep off

exit 0