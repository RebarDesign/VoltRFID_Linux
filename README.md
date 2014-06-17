VoltRFID_Linux
==============

Linux files for RFID reader installation

===============

17 Jun 2014 

(C) Elvis Adomnica , Sebastian Florian

Make sure nodejs is installed. 



==1==

Copy /voltLib files from USB

run as root :


chmod +x installRFID

./installRFID

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

Copy /readData from USB 




====

==4==

Create /etc/modprobe.d/blacklist-libnfc.conf

Write :

blacklist pn533
blacklist nfc

Save and close

sudo modprobe -r pn533 nfc

====

==5== 

from /readData

node index.js 


====

Scan chargers 



(C)

