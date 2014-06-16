VoltRFID_Linux
==============

Linux files for RFID reader installation

==1==
First make sure you have required compilation dependencies.
###
sudo apt-get update

sudo apt-get install libusb-dev libpcsclite-dev

sudo apt-get install libusb-0.1-4 libpcsclite1 libccid pcscd

sudo apt-get install pkg-config ( if not installed ) 


###
====

==2==

Using an ACR122 device with libnfc and without tag (e.g. to use NFCIP modes or
card emulation) needs yet another PCSC-lite tweak: You need to allow usage of
CCID Exchange command.  To do this, edit libccid_Info.plist configuration file
(usually /etc/libccid_Info.plist) and locate "<key>ifdDriverOptions</key>",
turn "<string>0x0000</string>" value into 0x0005 to allow CCID exchange and bogus devices (cf previous remark) and
restart pcscd daemon.

====

==3==

Copy libnfc from USB Stick

Configure Drivers
###
cd libnfc-1.7.1

./configure --with-drivers=acr122_pcsc

make 

sudo make install

====

==4==

Create /etc/modprobe.d/blacklist-libnfc.conf

Write :

blacklist pn533
blacklist nfc

Save and close

sudo modprobe -r pn533 nfc

====

Links

http://robospatula.blogspot.dk/2014/01/configure-install-libnfc-linux-PN532-breakout-board.html

https://code.google.com/p/libnfc/issues/detail?id=253

https://groups.google.com/forum/#!topic/libnfc-commits/rdb1Pfplzvc

http://www.libnfc.org/community/topic/668/solved-scl3711-interface-0-claimed-by-pn533-nfclist-sets-conf/

http://libnfc.googlecode.com/git/README

libnfc

/usr/local/lib libnfc.la  libnfc.a

/usr/bin/install








