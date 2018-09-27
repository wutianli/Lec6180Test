#!/bin/sh

#-----------------------------------------------------------------------------#
# iprenew TEST
#                                                  Li guang 2018.1.5
#-----------------------------------------------------------------------------#


LOG_FILE="/tmp/iprenew.log"
echo "[`basename $0`]: into iprenew test..............."
echo "================================================="

ifconfig eth0 192.168.100.22 up

echo "please press iprenew button then press letters 'o'!"

read -n1 test_name </dev/ttyS0

case $test_name in
        o) ifconfig eth0 > $LOG_FILE
		;;
esac

ifconfig eth0 > $LOG_FILE

 if grep "192.168.123.25" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [iprenew   ]  test                  OK"
    else
        echo "====   [iprenew   ]  test                  ERR"
		exit 1
 fi

	
exit 0
