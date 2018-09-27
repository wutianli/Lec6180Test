#!/bin/sh

#------------------------------------------------------------------------------#
# NTP
#
#------------------------------------------------------------------------------#
ifconfig eth0 192.168.123.25 up
route add default gw 192.168.123.254 eth0
echo "nameserver 192.168.123.254" > /etc/resolv.conf

i=0
while true; do
    i=`expr $i + 1`

    ntpdate ntp.ubuntu.com
    if [ $? -eq 0 ]; then
        hwclock -w
        echo "====   [`date`]"
        exit 0
    fi

    if [ $i -eq 3 ];then
        echo "====   [ntpdate ntp.ubuntu.com]   ERROR"
        exit 1
    fi
    beep 0x1
done

#------------------------------------------------------------------------------#
# End
#------------------------------------------------------------------------------#
