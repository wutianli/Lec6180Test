#!/bin/sh

#------------------------------------------------------------------------------#
# Product Initialize Script (from USB disc)
#
#    Test Hardware, copy images to FLASH, and then write MAC.
#------------------------------------------------------------------------------#

CUR_PATH=`pwd`
CONFIG="../config"
FILE_MAC_LIST="mac_list"
FILE_MAC_WRITE="mac_write"
FW_SETENV="../tools/fw_setenv"
show_mac_info ()
{
    SHOW_MAC=`ifconfig eth0 | grep HWaddr | awk '{print $5}'`
    echo "===="
    echo "====   [eth0  ]  $SHOW_MAC        OK"
    SHOW_MAC=`ifconfig eth1 | grep HWaddr | awk '{print $5}'`
    echo "====   [eth1  ]  $SHOW_MAC        OK"
    echo "===="
}

read_mac_from_config ()
{
    ETH0_LOCAL_MAC=`ifconfig eth0 | grep HWaddr | awk '{print $5}'`
    ETH1_LOCAL_MAC=`ifconfig eth1 | grep HWaddr | awk '{print $5}'`
    
    ETH0_MAC="`grep eth0_mac $CONFIG | cut -d "=" -f 2`"
    ETH1_MAC="`grep eth1_mac $CONFIG | cut -d "=" -f 2`"
    
    if [ "$ETH0_LOCAL_MAC" = "$ETH0_MAC" ];then
        if [ "$ETH1_LOCAL_MAC" = "$ETH1_MAC" ];then
            show_mac_info
            exit 0
        else
            echo "====   check eth1 Mac not match"
        fi
    else
        echo "====   check eth0 Mac not match"
    fi
}

check_mac_from_rules ()
{
    echo $1 | grep 00:90:0B > /dev/null
    if [ $? -eq "0" ];then
        echo $2 | grep 00:90:0B > /dev/null
        if [ $? -eq "0" ];then
            show_mac_info
            exit 0
        else
            echo "====   check eth1 Mac $2 not match"
        fi
    else
        echo "====   check eth0 Mac $1 not match"
    fi

}


read_mac_from_list ()
{
    #ETH0_LOCAL_MAC=`ifconfig eth0 | grep HWaddr | awk '{print $5}'`
    #ETH1_LOCAL_MAC=`ifconfig eth1 | grep HWaddr | awk '{print $5}'`
    
    #check_mac_from_rules $ETH0_LOCAL_MAC $ETH1_LOCAL_MAC
    
    MAC_RANGE_MAX="`grep mac_range $CONFIG | cut -d "=" -f 2 | cut -d "-" -f 2`"
    MAC_RANGE_MIN="`grep mac_range $CONFIG | cut -d "=" -f 2 | cut -d "-" -f 1`"

    if [ -e $FILE_MAC_WRITE ]; then
        MAC_RANGE_OFF=`cat $FILE_MAC_WRITE`
    else
        MAC_RANGE_OFF=0
    fi
    
    echo "====   Mac Range [ $MAC_RANGE_MIN - $MAC_RANGE_MAX ] Offset :$MAC_RANGE_OFF       ===="
    
    MAC_INDEX=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF`
    MAC_INDEX2=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 1`
	MAC_INDEX3=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 2`
	MAC_INDEX4=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 3`
	MAC_INDEX5=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 4`
	MAC_INDEX6=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 5`
	MAC_INDEX7=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 6`
	MAC_INDEX8=`expr $MAC_RANGE_MIN + $MAC_RANGE_OFF + 7`
    if [ $MAC_INDEX -ge $MAC_RANGE_MAX ]; then
        echo "===="
        echo "====   Out Of Range[ $MAC_RANGE_MIN - $MAC_RANGE_MAX ] index :$MAC_INDEX    ERROR"
        echo "===="
        exit 1
    fi
    ETH0_MAC="`grep mac_$MAC_INDEX"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
    ETH1_MAC="`grep mac_$MAC_INDEX2"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH2_MAC="`grep mac_$MAC_INDEX3"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH3_MAC="`grep mac_$MAC_INDEX4"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH4_MAC="`grep mac_$MAC_INDEX5"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH5_MAC="`grep mac_$MAC_INDEX6"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH6_MAC="`grep mac_$MAC_INDEX7"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	ETH7_MAC="`grep mac_$MAC_INDEX8"=" $FILE_MAC_LIST | cut -d "=" -f 2`"
	
}

#------------------------------------------------------------------------------#
# Mac Init --- Get Mac
#------------------------------------------------------------------------------#
# according to config write mac address
if grep "manual_mac=1" $CONFIG >/dev/null 2>&1; then
    echo "====   Into Manual Mac Check Mode            ===="
    read_mac_from_config
else
    echo "====   Into Mac list Check Mode              ===="
    read_mac_from_list
fi

#echo "====   Get Eth0 Mac :  $ETH0_MAC     ===="
#echo "====   Get Eth1 Mac :  $ETH1_MAC     ===="

#------------------------------------------------------------------------------#
#  Format 00:90:0B:22:9F:CA  ==>>  0x00,0x90,0x0B,0x22,0x9F,0xCA
#------------------------------------------------------------------------------#
#ETH1_MAC1=`echo $ETH1_MAC | awk -F: '{print $1}'`
#ETH1_MAC2=`echo $ETH1_MAC | awk -F: '{print $2}'`
#ETH1_MAC3=`echo $ETH1_MAC | awk -F: '{print $3}'`
#ETH1_MAC4=`echo $ETH1_MAC | awk -F: '{print $4}'`
#ETH1_MAC5=`echo $ETH1_MAC | awk -F: '{print $5}'`
#ETH1_MAC6=`echo $ETH1_MAC | awk -F: '{print $6}'`
#ETH1_MAC_FMT=0x$ETH1_MAC1,0x$ETH1_MAC2,0x$ETH1_MAC3,0x$ETH1_MAC4,0x$ETH1_MAC5,0x$ETH1_MAC6
#echo $ETH1_MAC_FMT

#------------------------------------------------------------------------------#
# Write eth Mac
#------------------------------------------------------------------------------#
cd $CUR_PATH
cp ../tools/fw_env.config /etc/
$FW_SETENV bootcmd 'run distro_bootcmd; run qspi_bootcmd; env exists secureboot && esbc_halt'
$FW_SETENV eth2addr $ETH0_MAC
$FW_SETENV eth3addr $ETH1_MAC
$FW_SETENV eth6addr $ETH2_MAC
$FW_SETENV eth1addr $ETH3_MAC
$FW_SETENV eth5addr $ETH4_MAC
$FW_SETENV eth4addr $ETH5_MAC
$FW_SETENV eth7addr $ETH6_MAC
$FW_SETENV ethaddr $ETH7_MAC
   

#------------------------------------------------------------------------------#
# Write mac_write
#------------------------------------------------------------------------------#
if grep "manual_mac=0" $CONFIG >/dev/null 2>&1; then
    MAC_RANGE_OFF=`expr $MAC_RANGE_OFF + 8`
    echo $MAC_RANGE_OFF > $FILE_MAC_WRITE
else
    echo 0 > $FILE_MAC_WRITE
fi
    sync
#------------------------------------------------------------------------------#
# End
#------------------------------------------------------------------------------#
