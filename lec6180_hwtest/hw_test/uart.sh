#!/bin/sh

#-----------------------------------------------------------------------------#
# uart test
#                                                   Li Guang 2018.1.2
#-----------------------------------------------------------------------------#

CONFIG_FILE="../config"    # Configuration file
LOG_FILE="/tmp/uart.log"


echo "[`basename $0`]: into uart test.................."
echo "================================================="
../tools/tty > $LOG_FILE


 if grep "uart test OK" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [uart   ]  test                  OK"
    else
        echo "====   [uart   ]  test                  ERR"
		exit 1
 fi

	
exit 0
