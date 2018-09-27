#!/bin/sh

#-----------------------------------------------------------------------------#
# temp TEST
#                                                  Li guang 2018.1.5
#-----------------------------------------------------------------------------#

LOG_FILE="/tmp/temp.log"


echo "[`basename $0`]: into temp test.................."
echo "================================================="
temp > $LOG_FILE


 if grep "temp2" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [temp   ]  test                  OK"
    else
        echo "====   [temp   ]  test                  ERR"
		exit 1
 fi

	
exit 0
