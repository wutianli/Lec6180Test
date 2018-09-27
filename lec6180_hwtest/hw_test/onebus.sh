#!/bin/sh

#-----------------------------------------------------------------------------#
# one_bus TEST
#                                                  Li guang 2018.1.5
#-----------------------------------------------------------------------------#

LOG_FILE="/tmp/onebus.log"


echo "[`basename $0`]: into onebus test................"
echo "================================================="
w1 > $LOG_FILE


 if grep "CRC good" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [onebus   ]  test                OK"
    else
        echo "====   [onebus   ]  test                ERR"
		exit 1
 fi

	
exit 0
