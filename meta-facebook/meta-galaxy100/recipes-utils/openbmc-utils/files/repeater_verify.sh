#!/bin/bash

. /usr/local/bin/openbmc-utils.sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin

i2c_read() {
	local count=0
	while [ $count -lt 10 ]; do
		temp=`i2cget -f -y $1 $2 $3`
		if [ $? -eq 0 ]; then
			return $(($temp))
		fi
		count=$(($count+1))
		usleep 11000
	done
	return 1
}

#$1: bus
#$2: addr
#$3: reg
#$4: value
#$5: mask
i2c_read_verify() {
	((val=$4))
	((mask=$5))
	#echo -n "i2c read $1 $2 $3  "
	i2c_read $1 $2 $3 2> /dev/null
	temp=$?
	printf "Verify bus:%d addr:0x%02x reg:0x%02x value:0x%02x (expect 0x%02x)   " $1 $2 $3 $(($temp&$mask)) $4
	#echo -n "=$temp val=$val    "
	if [ $temp -eq 1 ]; then
		echo -e "\033[41;37m[FAIL]\033[0m"
		return 1
	fi
	((ret=temp))
	#echo -n "ret=$ret  mask=$mask  "
	if (((($ret & $mask)) == (($val)))); then
		echo -e "\033[42;37m[PASS]\033[0m"
		return 0
	else
		echo -e "\033[41;37m[FAIL]\033[0m"
		return 1
	fi
	return 1
}

repeater_config() {
	((val=$(i2cget -f -y 12 0x31 0x3 2> /dev/null | head -n 1)))
	((left=$(i2cget -f -y 12 0x31 0x5 2> /dev/null | head -n 1)))
	if [ $val -lt 8  -a $left -eq 0 ] || [ $val -ge 8 ]; then
		i2c_read_verify 0 0x58 0x01 0x0 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x0a 0x0 0x0
		usleep 11000
		i2c_read_verify 0 0x58 0x06 0x18 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x2c 0x00 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x33 0x00 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x3a 0x00 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x41 0x00 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x2d 0xa8 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x34 0xa8 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x3b 0xa9 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x42 0xa9 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x2e 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x35 0x01 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x3c 0x02 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x43 0x02 0x1f
		usleep 11000

		i2c_read_verify 0 0x58 0x0f 0x0 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x16 0x0 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x1d 0x0 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x24 0x0 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x10 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x17 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x1e 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x25 0xaf 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x11 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x18 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x1f 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x26 0x0 0x1f
		usleep 11000
	else

		i2c_read_verify 0 0x58 0x01 0x0 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x0a 0x0 0x0
		usleep 11000
		i2c_read_verify 0 0x58 0x06 0x18 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x2c 0x04 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x33 0x04 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x3a 0x00 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x41 0x00 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x2d 0xab 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x34 0xab 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x3b 0xab 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x42 0xab 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x2e 0x01 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x35 0x01 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x3c 0x02 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x43 0x02 0x1f
		usleep 11000

		i2c_read_verify 0 0x58 0x0f 0x01 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x16 0x01 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x1d 0x01 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x24 0x01 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x10 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x17 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x1e 0xaf 0xff
		usleep 11000
		i2c_read_verify 0 0x58 0x25 0xaf 0xff
		usleep 11000

		i2c_read_verify 0 0x58 0x11 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x18 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x1f 0x0 0x1f
		usleep 11000
		i2c_read_verify 0 0x58 0x26 0x0 0x1f
		usleep 11000
	fi
}

repeater_config
