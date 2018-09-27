#!/bin/sh

#-----------------------------------------------------------------------------#
# RTC TEST
#                                       Li Guang 2016.5.3
#-----------------------------------------------------------------------------#
PRODUCTION_CONFIG_FILE="../config"
DATE=101016002019

echo "[`basename $0`]: into rtc test..................."
echo "================================================="

#-----------------------------------------------------------------------------#
# Write Rtc 
#-----------------------------------------------------------------------------#
SETING_TIME=0
#if grep test_step=1 $PRODUCTION_CONFIG_FILE > /dev/null 2>&1; then
    echo "[`basename $0`]: ** Write Rtc Chip at Mainboard Test **"
    date $DATE
    hwclock -w
    SETING_TIME=1
#fi

#-----------------------------------------------------------------------------#
# Read Rtc 
#-----------------------------------------------------------------------------#
YEAR="`hwclock | awk '{print $4}'`"
if [ "$YEAR" -ge "2019" ];then
    echo "[`basename $0`]: Read Rtc                    OK"
else
    echo "[`basename $0`]: Read Rtc                    ERR"
    exit 1
fi

#-----------------------------------------------------------------------------#
# Check Rtc Running 
#-----------------------------------------------------------------------------#
#DEF_TIME="Sun Jan  1 00:00:04 2016  0.000000 seconds"
DEF_TIME="SunJan100:00:0"
CURRENT_TIME="/tmp/current.time"

if [ "$SETING_TIME" -eq "0" ];then
    T1="`hwclock | awk '{print $1}'`"
    T2="`hwclock | awk '{print $2}'`"
    T3="`hwclock | awk '{print $3}'`"
    T4="`hwclock | awk '{print $4}'`"
    echo $T1$T2$T3$T4 > $CURRENT_TIME
    
    if grep $DEF_TIME $CURRENT_TIME;then
    echo "[`basename $0`]: Running Rtc                  ERR"
    exit 1
    else
    echo "[`basename $0`]: Running Rtc                  OK"
    fi
fi
rm -fr $CURRENT_TIME
exit 0
