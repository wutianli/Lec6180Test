#!/bin/sh

#-----------------------------------------------------------------------------#
#  update
#                                               Li Guang 2016.5.3
#-----------------------------------------------------------------------------#
# stop watchdog
#echo V > /dev/watchdog
#flash_write
#flash_eraseall /dev/mtd8

UPDATE_PATH="/tmp/fw/bin/"
ROOTFS_BIN=$UPDATE_PATH/rootfs.cramfs
ROOTFS_MD5=$UPDATE_PATH/rootfs.md5
KERNEL_BIN=$UPDATE_PATH/uImage
KERNEL_MD5=$UPDATE_PATH/uImage.md5
U_BOOT_BIN=$UPDATE_PATH/u-boot.bin
U_BOOT_MD5=$UPDATE_PATH/u-boot.md5
BOOT_S_BIN=$UPDATE_PATH/boots
BOOT_S_MD5=$UPDATE_PATH/boots.md5

# update rootfs
if [ -f $ROOTFS_BIN ] && [ -f $ROOTFS_MD5 ];then
    TMP1_MD5=`md5sum $ROOTFS_BIN | awk '{print $1}'`
    TMP2_MD5=`cat $ROOTFS_MD5 | awk '{print $1}'`
    if [ $TMP1_MD5 = $TMP2_MD5 ]; then
        flash_write -c $ROOTFS_BIN /dev/mtd10
        if [ $? -eq 0 ];then
            echo "[`basename $0`] Rootfs Version the latest!"
        else
            echo "[`basename $0`] Rootfs Update Now ... ..."
            flash_eraseall /dev/mtd10
            flash_write $ROOTFS_BIN /dev/mtd10
        fi
    else
        echo "[`basename $0`] Rootfs MD5 Check Error"
    fi
else
    echo "[`basename $0`] No Found Rootfs File For Updated"
fi

# update kernel
if [ -f $KERNEL_BIN ] && [ -f $KERNEL_MD5 ];then
    TMP1_MD5=`md5sum $KERNEL_BIN | awk '{print $1}'`
    TMP2_MD5=`cat $KERNEL_MD5 | awk '{print $1}'`
    if [ $TMP1_MD5 = $TMP2_MD5 ]; then
        flash_write -c $KERNEL_BIN /dev/mtd9
        if [ $? -eq 0 ];then
            echo "[`basename $0`] Kernel Version the latest!"
        else
            echo "[`basename $0`] Kernel Update Now ... ..."
            flash_eraseall /dev/mtd9
            flash_write $KERNEL_BIN /dev/mtd9
        fi
    else
        echo "[`basename $0`] Kernel MD5 Check Error"
    fi
else
    echo "[`basename $0`] No Found Kernel File For Updated"
fi

# update Uboot
if [ -f $U_BOOT_BIN ] && [ -f $U_BOOT_MD5 ];then
    TMP1_MD5=`md5sum $U_BOOT_BIN | awk '{print $1}'`
    TMP2_MD5=`cat $U_BOOT_MD5 | awk '{print $1}'`
    if [ $TMP1_MD5 = $TMP2_MD5 ]; then
        flash_write -c $U_BOOT_BIN /dev/mtd8
        if [ $? -eq 0 ];then
            echo "[`basename $0`] Uboot Version the latest!"
        else
            echo "[`basename $0`] Uboot Update Now ... ..."
            flash_eraseall /dev/mtd8
            flash_write $U_BOOT_BIN /dev/mtd8
        fi
    else
        echo "[`basename $0`] Uboot MD5 Check Error"
    fi
else
    echo "[`basename $0`] No Found Uboot File For Updated"
fi

# update boots
if [ -f $BOOT_S_BIN ] && [ -f $BOOT_S_MD5 ];then
    TMP1_MD5=`md5sum $BOOT_S_BIN | awk '{print $1}'`
    TMP2_MD5=`cat $BOOT_S_MD5 | awk '{print $1}'`
    if [ $TMP1_MD5 = $TMP2_MD5 ]; then
        flash_write -c $BOOT_S_BIN /dev/mtd7
        if [ $? -eq 0 ];then
            echo "[`basename $0`] Boots Version the latest!"
        else
            echo "[`basename $0`] Boots Update Now ... ..."
            flash_eraseall /dev/mtd7
            flash_write $BOOT_S_BIN /dev/mtd7
        fi
    else
        echo "[`basename $0`] Boots MD5 Check Error"
    fi
else
    echo "[`basename $0`] No Found Boots File For Updated"
fi
