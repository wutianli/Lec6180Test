#!/bin/sh

#-----------------------------------------------------------------------------#
# b_ma TEST
#                                                  Li guang 2018.1.5
#-----------------------------------------------------------------------------#


LOG_FILE="/tmp/b_ma.log"
echo "[`basename $0`]: into b_ma test.................."
echo "================================================="

ifconfig bm0 147.253.148.2 up


/mnt/lec6180_hwtest/tools/ptpd2 -b bm0 -g -c -L > $LOG_FILE &

sleep 16

 if grep "stat:   OK" $LOG_FILE >/dev/null 2>&1; then
        echo "====   [b_ma   ]  test                  OK"
		hwclock -w
    else
        echo "====   [b_ma   ]  test                  ERR"
		exit 1
 fi

exit 0
