#!/bin/sh

#------------------------------------------------------------------------------#
# Config Eth1 Enable                                  Li Guang 2018.1.3
#------------------------------------------------------------------------------#
CONFIG_FILE="./config"    # Configuration file
FLAG="0"

if [ "$FLAG" -eq "1" ];then

echo "Test mainboard please input: a"
echo "Init system    please input: b"
echo "Test hardware  please input: c"
echo "Test wdg       please input: d"
echo "Init mac       please input: e"

read -n1 test_name </dev/ttyS0

case $test_name in
        a) TEST_PATH="mainboard"
		RUN_SH="mainboard.sh"
		echo "[`basename $0`]: Test mainboard ..."
		;;
		
		b) TEST_PATH="init"
		RUN_SH="init.sh"
		echo "[`basename $0`]: Init system ..."
		;;
		
        c) TEST_PATH="hw_test"
		RUN_SH="hw_test.sh"
		echo "[`basename $0`]: Test hardware ..."
		;;
		
        d)TEST_PATH="hw_test"
		RUN_SH="wdg.sh"
		echo "[`basename $0`]: Test wdg ..."
		;;
		
		e)TEST_PATH="init"
		RUN_SH="mac.sh"
		echo "[`basename $0`]: Init mac ..."
		;;
esac
fi


if [ "$FLAG" -eq "0" ];then
TEST_PATH="hw_test"
RUN_SH="hw_test.sh"
echo "[`basename $0`]: Test hardware ..."
fi

cd /mnt/lec6180_hwtest/
cd $TEST_PATH
sh $RUN_SH
