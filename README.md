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
Optional 

Go to /etc/nfc/

wget https://github.com/RebarDesign/VoltRFID_Linux/blob/master/devices.d/pn532_uart_on_rpi.conf

====
==4==
Go to /etc/blacklist/
wget https://github.com/RebarDesign/VoltRFID_Linux/blob/master/blacklist-libnfc.conf

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








