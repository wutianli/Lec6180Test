#!/bin/sh

#------------------------------------------------------------------------------#
# Product Initialize Script (from USB disc)
#
#    Test Hardware, copy images to FLASH, and then write MAC.
#------------------------------------------------------------------------------#

CUR_PATH=`pwd`
CONFIG="../config"

#------------------------------------------------------------------------------#
# Mac Init
#------------------------------------------------------------------------------#
cd $CUR_PATH
if sh ./mac.sh; then
    echo "[`basename $0`]: Mac Init OK"
else
    echo "[`basename $0`]: Mac Init Error"
    exit 1
fi


#------------------------------------------------------------------------------#
# HW Test
#------------------------------------------------------------------------------#
cd ../hw_test/
if sh ./hw_test.sh; then
    echo "[`basename $0`]: HW Test OK"
else
    echo "[`basename $0`]: HW Test Error"
    exit 1
fi

#------------------------------------------------------------------------------#
# Server Init
#------------------------------------------------------------------------------#
#cd $CUR_PATH
#if sh ./server.sh; then
#    echo "[`basename $0`]: Server Init OK"
#else
#    echo "[`basename $0`]: Server Init Error"
#    exit 1
#fi

#------------------------------------------------------------------------------#
# Flash Init
#------------------------------------------------------------------------------#
#cd $CUR_PATH
#if sh ./flash; then
#    echo "[`basename $0`]: Flash Init OK"
#else
#    echo "[`basename $0`]: Flash Init Error"
#    exit 1
#fi

#------------------------------------------------------#
#  Sync UTC Time
#------------------------------------------------------#
#cd $CUR_PATH
#if sh ./ntpdate.sh; then
#    echo "[`basename $0`]: ntpdate Init OK"
#else
#    echo "[`basename $0`]: ntpdate Init Error"
#    exit 1
#fi

#------------------------------------------------------#
#  Remove Aging Script
#------------------------------------------------------#
#rm -fr /usr/local/full_cpu.sh /usr/local/rc
sync

#------------------------------------------------------#
#  Watch Dog Test
#------------------------------------------------------#
#cd $CUR_PATH
#cd ../hw_test/
#beep 0x5555
#sh wdg.sh

#------------------------------------------------------------------------------#
# End
#------------------------------------------------------------------------------#
