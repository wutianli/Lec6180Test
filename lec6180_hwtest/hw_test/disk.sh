#!/bin/sh

#-----------------------------------------------------------------------------#
# disk TEST
#                                                  Li guang 2018.7.19
#-----------------------------------------------------------------------------#


LOG_FILE="/tmp/disk.log"
echo "[`basename $0`]: into disk test.................."
echo "================================================="


fdisk -l /dev/sdc > $LOG_FILE &

sleep 2

 if grep "/dev/sdc" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [disk   ]  test                  OK"
    else
        echo "====   [disk   ]  test                  ERR"
		exit 1
 fi

exit 0
