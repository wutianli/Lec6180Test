#!/bin/sh

#-----------------------------------------------------------------------------#
# EEPROM TEST
# ZHANG WEI 2009.01.15
#-----------------------------------------------------------------------------#
PRODUCTION_CONFIG_FILE="../config"
EEP_R="../tools/eeprom -r"
EEP_W="../tools/eeprom -w"
EEP_C="../tools/eeprom -c"

#-----------------------------------------------------------------------------#
# Write E2PROM 
#-----------------------------------------------------------------------------#
SETING_EEP=0
if grep test_step=1 $PRODUCTION_CONFIG_FILE > /dev/null 2>&1; then
    echo "[`basename $0`]: ** Write EEPROM Chip at Mainboard Test **"
    $EEP_C
    $EEP_W 0 at24c08
    if [ $? -eq "0" ];then
        echo "[`basename $0`]: Write EEPROM OK"
    else
        echo "[`basename $0`]: Write EEPROM Error"
        exit 1
    fi
    SETING_EEP=1
fi


#-----------------------------------------------------------------------------#
# Read E2PROM 
#-----------------------------------------------------------------------------#
if [ "$SETING_EEP" -eq "0" ];then
    VALUE=`../tools/eeprom -r`
    if [ "$VALUE" == "at24c08" ];then
        echo "[`basename $0`]: Read EEPROM OK"
    else
        echo "[`basename $0`]: Read EEPROM Error"
        exit 1
    fi
fi
exit 0
