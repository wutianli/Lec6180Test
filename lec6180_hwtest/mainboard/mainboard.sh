#!/bin/sh

#-----------------------------------------------------------------------------#
# mainboard test script
#																									ZHANG WEI 2012.04.10
#-----------------------------------------------------------------------------#
CUR_PATH=`pwd`


#------------------------------------------------------#
# Hardware Test
#------------------------------------------------------#
cd ../hw_test/
sh hw_test.sh
if [ $? -ne 0 ]; then
    echo "[`basename $0`] hardware test Failed!"
    exit 1
fi
echo "[`basename $0`] hardware test OK"

#------------------------------------------------------------------------------#
# Mac Init
#------------------------------------------------------------------------------#
#cd ../init/
#if sh ./mac.sh; then
#    echo "[`basename $0`]: Mac Init OK"
#else
#    echo "[`basename $0`]: Mac Init Error"
#    exit 1
#fi

#------------------------------------------------------#
#  Write Aging Script
#------------------------------------------------------#
cd $CUR_PATH
cp rc  full_cpu.sh  /usr/local/
sync

#------------------------------------------------------#
#  Watch Dog Test
#------------------------------------------------------#
#cd ../hw_test/
#sh wdg.sh

echo "[`basename $0`] Finish MainBoard Test"
exit 0
