#!/bin/sh

#-----------------------------------------------------------------------------#
# JUST FOR TEST
#                                           Li Guang 2018.1.3
#-----------------------------------------------------------------------------#

CUR_PATH=`pwd`
UART="uart"
NET="net"
RTC="rtc"
LED_DO="led_do"
USB_SATA="usb_sata"
TEMP="temp"
ONEBUS="onebus"
PWR="pwr"
IPRENEW="iprenew"
B_MA="b_ma"
DISK="disk"

SHOW_TEST_STATUS="/tmp/status"

FLAG="0"

#------------------------------------------------------#
#  DELETE DISPLAY STATUS FILES
#------------------------------------------------------#

rm -fr $SHOW_TEST_STATUS
touch  $SHOW_TEST_STATUS


echo "net  test please input: 1"
echo "uart test please input: 2"
echo "rtc  test please input: 3"
echo "temp test please input: 4"
echo "pwr  test please input: 5"
echo "onebus   test please input: 6"
echo "iprenew  test please input: 7"
echo "b_ma     test please input: 8"
echo "led_do   test please input: 9"
echo "usb&sata test please input: a"
echo "Please  press letters 'Enter' if test all!"

read test_name </dev/ttyS0

case $test_name in
		1) 
		FLAG="1"
		sh $NET.sh
		echo "$NET=$?" >> $SHOW_TEST_STATUS
		;;
		
        2) 
		FLAG="1"
		sh $UART.sh
		echo "$UART=$?" >> $SHOW_TEST_STATUS
		;;
		
        3) 
		FLAG="1"
		sh $RTC.sh
		echo "$RTC=$?" >> $SHOW_TEST_STATUS
		;;
		
		4) 
		FLAG="1"
		sh $TEMP.sh
		echo "$TEMP=$?" >> $SHOW_TEST_STATUS
		;;
		
		5) 
		FLAG="1"
		sh $PWR.sh
		echo "$PWR=$?" >> $SHOW_TEST_STATUS
		;;
		
		6)  
		FLAG="1"
		sh $ONEBUS.sh
		echo "$ONEBUS=$?" >> $SHOW_TEST_STATUS
		;;
		
		7) 
		FLAG="1"
		sh $IPRENEW.sh
		echo "$IPRENEW=$?" >> $SHOW_TEST_STATUS
		;;
		
		8) 
		FLAG="1"
		sh $B_MA.sh
		echo "$B_MA=$?" >> $SHOW_TEST_STATUS
		;;
		
		9) 
		FLAG="1"
		sh $LED_DO.sh
		;;
		
		a) 
		FLAG="1"
		sh $USB_SATA.sh
		;;

		
esac

   if [ $FLAG -eq "0" ]; then
        #sh $UART.sh
		#echo "$UART=$?" >> $SHOW_TEST_STATUS
		sh $NET.sh
		echo "$NET=$?" >> $SHOW_TEST_STATUS
		sh $RTC.sh
		echo "$RTC=$?" >> $SHOW_TEST_STATUS
		sh $TEMP.sh
		echo "$TEMP=$?" >> $SHOW_TEST_STATUS
		sh $PWR.sh
		echo "$PWR=$?" >> $SHOW_TEST_STATUS
		sh $ONEBUS.sh
		echo "$ONEBUS=$?" >> $SHOW_TEST_STATUS
		sh $B_MA.sh
		echo "$B_MA=$?" >> $SHOW_TEST_STATUS
		sh $DISK.sh
		echo "$DISK=$?" >> $SHOW_TEST_STATUS
		#sh $IPRENEW.sh
		#echo "$IPRENEW=$?" >> $SHOW_TEST_STATUS
		sh $LED_DO.sh
		
		sh show_test.sh
		exit $?
    fi
		

#------------------------------------------------------#
#  SHOW TEST
#------------------------------------------------------#
exit 0
