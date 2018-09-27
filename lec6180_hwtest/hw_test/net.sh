#!/bin/sh

#-----------------------------------------------------------------------------#
#  Test Network
#                                               Li Guang 2018.1.3
#-----------------------------------------------------------------------------#

CONFIG_FILE="../config"    # Configuration file

LOG_FILE1="/tmp/net_eth1"
LOG_FILE2="/tmp/net_eth2"
LOG_FILE3="/tmp/net_eth3"
LOG_FILE4="/tmp/net_eth4"
LOG_FILE5="/tmp/net_eth5"
LOG_FILE6="/tmp/net_eth6"
LOG_FILE7="/tmp/net_eth7"
LOG_FILE8="/tmp/net_eth8"
IPERF_FILE="../tools/net_test"
ERR_FLAG="0"


echo "[`basename $0`]: into net test..................."
echo "================================================="
ifconfig eth1 up
ifconfig eth2 up
ifconfig eth3 up
ifconfig eth4 up
ifconfig eth5 up
ifconfig eth6 up
ifconfig eth7 up
ifconfig eth8 up

sleep 5

$IPERF_FILE -mt 1 2 > /dev/null &
$IPERF_FILE -mt 3 4 > /dev/null &
$IPERF_FILE -mt 5 6 > /dev/null &
$IPERF_FILE -mt 7 8 > /dev/null &
  
sleep 8
  
 if grep "error" $LOG_FILE1 >/dev/null 2>&1; then
        echo "====   [eth1]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth1]  test                  OK"
 fi

 if grep "error" $LOG_FILE2 >/dev/null 2>&1; then
        echo "====   [eth2]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth2]  test                  OK"
 fi

 if grep "error" $LOG_FILE3>/dev/null 2>&1; then
        echo "====   [eth3]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth3]  test                  OK"
 fi

 if grep "error" $LOG_FILE4 >/dev/null 2>&1; then
        echo "====   [eth4]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth4]  test                  OK"
 fi
 
  if grep "error" $LOG_FILE5 >/dev/null 2>&1; then
        echo "====   [eth5]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth5]  test                  OK"
 fi
 
  if grep "error" $LOG_FILE6 >/dev/null 2>&1; then
        echo "====   [eth6]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth6]  test                  OK"
 fi
 
  if grep "error" $LOG_FILE7 >/dev/null 2>&1; then
        echo "====   [eth7]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth7]  test                  OK"
 fi
 
  if grep "error" $LOG_FILE8 >/dev/null 2>&1; then
        echo "====   [eth8]  test                  ERR"
		ERR_FLAG="1"
    else
        echo "====   [eth8]  test                  OK"
 fi
 
  if [ $ERR_FLAG -eq "1" ]; then
        echo "================================================="
        echo "[`basename $0`]: NET Test            Error!"
		echo "================================================="
        exit 1   
  fi

echo "================================================="
echo "[`basename $0`]: NET Test               OK!"
echo "=================================================" 

exit 0

