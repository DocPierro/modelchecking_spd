#!/bin/bash

#  Script.sh
#  Cosmos
#
#  Created by Benoît Barbot on 05/02/2015.
#

LPATH=sketchArduino/lib/SimClient
SEDCMD=s/.hpp/.h/g

rm -rf sketchArduino
mkdir -p sketchArduino
cd sketchArduino
ino init
rm -rf src/sketch.ino
cd ..
mkdir -p $LPATH
touch $LPATH/string
sed -e $SEDCMD Event.hpp > $LPATH/Event.h
sed -e $SEDCMD Event.cpp > $LPATH/Event.cpp
sed -e $SEDCMD EventsQueueLight.hpp > $LPATH/EventsQueueLight.h
sed -e $SEDCMD EventsQueueLight.cpp > $LPATH/EventsQueueLight.cpp
sed -e $SEDCMD spnLight.hpp > $LPATH/spnLight.h
sed -e $SEDCMD spn.cpp > $LPATH/spn.cpp
sed -e $SEDCMD SimLight.hpp > $LPATH/SimLight.h
sed -e $SEDCMD SimLight.cpp > $LPATH/SimLight.cpp
sed -e $SEDCMD markingImpl.hpp > $LPATH/markingImpl.h
sed -e $SEDCMD macro.h > $LPATH/macro.h
sed -e $SEDCMD Print.h > $LPATH/Print.h
cp Timer1.h $LPATH/Timer1.h
cp Timer1.c $LPATH/Timer1.c
echo "#include \"spnLight.h\"\n" $LPATH/magic.h
sed -e 's/return \("[^"]*"\);/print(\1);break;/g' -e 's/return to_string2(v)/print((TR_PL_ID)v);break;/g' -e 's/ string / void /g' -e $SEDCMD magic.hpp >> $LPATH/magic.h

cp sketchArduino.ino sketchArduino/src/sketch.ino
cp -r $LPATH ~/Documents/Arduino/libraries

cd sketchArduino


SERIAL_FIO=`ls /dev/tty.usbserial-* /dev/ttyUSB* 2>/dev/null`
#SERIAL_UNO=`ls /dev/tty.usbmodemfd* 2>/dev/null`
#SERIAL_BLEND=`ls /dev/tty.usbmodem* /dev/ttyACM* 2>/dev/null`

if [[ -n $SERIAL_BLEND ]]; then
    BOARD=blendmicro8
    SERIAL=$SERIAL_BLEND
    echo "Blend micro detected on "$SERIAL
elif [[ -n $SERIAL_UNO ]]; then
    BOARD=uno
    SERIAL=$SERIAL_UNO
    echo "Uno detected on "$SERIAL
else
    BOARD=fio
    SERIAL=$SERIAL_FIO
    echo "Fio detected on "$SERIAL
fi

ino build -m $BOARD
#readserial $SERIAL 1200 reset

ino upload -m $BOARD -p $SERIAL

rm arduino

cd ../..
ln -s $SERIAL arduino