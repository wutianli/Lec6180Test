#!/bin/sh

#-----------------------------------------------------------------------------#
# CF test
#                                       Zhang Wei 2008.10.16
#-----------------------------------------------------------------------------#
echo "[`basename $0`]: into cf test"

#-----------------------------------------------------------------------------#
# Load CF Card 
#-----------------------------------------------------------------------------#
MOD_PATH=/lib/modules/2.6.30/drivers/
insmod ${MOD_PATH}pata_at91.ko
sleep 2

mount -t vfat /dev/sdb /mnt/cfcard
if [ $? -ne "0" ];then
    mount -t vfat /dev/sdb1 /mnt/cfcard
    if [ $? -ne "0" ];then
        echo "[`basename $0`]: Mount CF Card Error"
        exit 1
    fi
fi

#-----------------------------------------------------------------------------#
# Write CF Card 
#-----------------------------------------------------------------------------#
echo "cf filesystem test" > /mnt/cfcard/test
if [ $? -ne "0" ];then
    echo "[`basename $0`]: Write CF Card Error"
    exit 1
fi

umount /mnt/cfcard
echo "[`basename $0`]: CF Card Test Ok"
exit 0
