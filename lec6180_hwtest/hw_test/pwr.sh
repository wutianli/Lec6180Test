#!/bin/sh

#-----------------------------------------------------------------------------#
# pwr TEST
#                                                  Li guang 2018.1.5
#-----------------------------------------------------------------------------#

LOG_FILE="/tmp/pwr.log"
echo "[`basename $0`]: into pwr test..................."
echo "Please open two power supplies then press letters 'Enter'!"
echo "================================================="

read test_name </dev/ttyS0
sleep 4

case $test_name in
        o) 
	gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
	gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr2   ]  test                  OK"
	fi
		;;
esac

gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr2   ]  test                  OK"
	fi
	
echo "Please turn off the power 1 then press letters 'Enter'!"
echo "================================================="

read test_name </dev/ttyS0
sleep 4

case $test_name in
        o) 
	gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
	gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr2   ]  test                  OK"
	fi
		;;
esac

gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr2   ]  test                  OK"
	fi

echo "Please open the power 1 & turn off the power 2 then press letters 'Enter'!"
echo "================================================="

read test_name </dev/ttyS0
sleep 4

case $test_name in
        o) 
	gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
	gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
    else
        echo "====   [pwr2   ]  test                  OK"
	fi
		;;
esac

gpio -get pwr1 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr1   ]  test                  ERR"
		exit 1
    else
        echo "====   [pwr1   ]  test                  OK"
	fi
 
gpio -get pwr2 > $LOG_FILE

	if grep "0" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [pwr2   ]  test                  ERR"
    else
        echo "====   [pwr2   ]  test                  OK"
	fi

exit 0
