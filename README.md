VoltRFID_Linux
==============

Linux files for RFID reader installation

==1==
First make sure you have required compilation dependencies.
###
sudo apt-get install libusb-dev libpcsclite-dev
###
====

==2==
Download and extract libnfc archive
###
wget https://github.com/RebarDesign/VoltRFID_Linux/tree/master/libnfc-1.7.1
###
====

==3==

Using an ACR122 device with libnfc and without tag (e.g. to use NFCIP modes or
card emulation) needs yet another PCSC-lite tweak: You need to allow usage of
CCID Exchange command.  To do this, edit libccid_Info.plist configuration file
(usually /etc/libccid_Info.plist) and locate "<key>ifdDriverOptions</key>",
turn "<string>0x0000</string>" value into 0x0001 to allow CCID exchange or
0x0005 to allow CCID exchange and bogus devices (cf previous remark) and
restart pcscd daemon.

====
==4==
Go to /etc/blacklist/
wget https://github.com/RebarDesign/VoltRFID_Linux/blob/master/blacklist-libnfc.conf

Then, restart or unplug the device, unload modules (sudo modprobe -r pn533 nfc), then re-plug the device.

====


==5==
Configure Drivers
###
cd libnfc-1.7.1

./configure --with-drivers=acr122_pcsc

make 

sudo make install

###
====
Links

http://robospatula.blogspot.dk/2014/01/configure-install-libnfc-linux-PN532-breakout-board.html

https://code.google.com/p/libnfc/issues/detail?id=253

https://groups.google.com/forum/#!topic/libnfc-commits/rdb1Pfplzvc

http://www.libnfc.org/community/topic/668/solved-scl3711-interface-0-claimed-by-pn533-nfclist-sets-conf/

http://libnfc.googlecode.com/git/README








