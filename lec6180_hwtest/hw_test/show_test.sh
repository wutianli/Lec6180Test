#!/bin/sh

#-----------------------------------------------------------------------------#
# show test
#                                               Li Guang 2018.1.5
#-----------------------------------------------------------------------------#

PRODUCTION_CONFIG_FILE="../config"
STATUS="/tmp/status"
SHOW_TEST_STATUS="/tmp/status"
FLAG="0"

#-----------------------------------------------------------------------------#
# display the detail
#-----------------------------------------------------------------------------#


echo ""
echo "================================================="

    if grep "net=1" $STATUS >/dev/null 2>&1; then
        echo "====   [Net    ]  test                  Error"
        FLAG="1"
    else
        echo "====   [Net    ]  test                     Ok"
    fi

    if grep "rtc=1" $STATUS >/dev/null 2>&1; then
        echo "====   [Rtc    ]  test                  Error"
        FLAG="1"
    else
        echo "====   [Rtc    ]  test                     Ok"
    fi
	
	if grep "temp=1" $STATUS >/dev/null 2>&1; then
        echo "====   [Temp   ]  test                  Error"
        FLAG="1"
    else
        echo "====   [Temp   ]  test                     Ok"
    fi
	
	if grep "onebus=1" $STATUS >/dev/null 2>&1; then
        echo "====   [Onebus ]  test                  Error"
        FLAG="1"
    else
        echo "====   [Onebus ]  test                     Ok"
    fi
	
	if grep "pwr=1" $STATUS >/dev/null 2>&1; then
        echo "====   [Pwr    ]  test                  Error"
        FLAG="1"
    else
        echo "====   [Pwr    ]  test                     Ok"
    fi
	
	
#	if grep "iprenew=1" $STATUS >/dev/null 2>&1; then
#        echo "====   [Iprenew]  test                  Error"
#        FLAG="1"
#    else
#        echo "====   [Iprenew]  test                     Ok"
#    fi
	
	
	if grep "b_ma=1" $STATUS >/dev/null 2>&1; then
        echo "====   [B_ma   ]  test                  Error"
       FLAG="1"
    else
        echo "====   [B_ma   ]  test                     Ok"
    fi
	
	if grep "disk=1" $STATUS >/dev/null 2>&1; then
        echo "====   [disk   ]  test                  Error"
       FLAG="1"
    else
        echo "====   [disk   ]  test                     Ok"
    fi

	

hwclock -r
#rm -rf /usr/local/lec6180_net/

    if [ $FLAG -eq "1" ]; then
        echo "================================================="
        echo "[`basename $0`]: Hdware Test Error!!"
		echo "================================================="
        gpio -beep on
		sleep 3
		gpio -beep off
        exit 1
    fi
    
echo "================================================="
echo "[`basename $0`]: Hdware Test passed !!"
echo "================================================="

exit 0


