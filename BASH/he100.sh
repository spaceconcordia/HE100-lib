#!/bin/bash

# select and configure serial device
PORT=/dev/ttyUSB0
stty -F $PORT raw speed 9600 cs8 -ignpar -cstopb -echo
sleep 0.1

#send noop
printf $'\x48\x65\x10\x01\x00\x00\x11\x43\x00\x00' > $PORT
#cat /dev/ttyS2 | hex dump -Cv &
#cat /home/code/testsend.dat > /dev/ttyS2

#send message 'Hello'
printf $'\x48\x65\x10\x03\x00\x05\x18\x4e\x48\x65\x6c\x6c\x6f\xf5\x8c' > $PORT

# listen and print
while [ 1 ]; do
#    cat /dev/ttyUSB0
  READ=`dd if=$PORT count=1`
  echo $READ
done
