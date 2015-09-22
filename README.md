# RFID Reader
### Development of a custom RFID Reader script, library and USB Driver for a Debian Server optimized for speed reading.

> GitHub repository: [https://github.com/RebarDesign/VoltRFID_Linux](https://github.com/RebarDesign/VoltRFID_Linux)

Technologies used: Node.js, Express, MySQL, Socket.io, Angular.js, Twitter Bootstrap, C, Linux system calls.

## Components

Components: 
1. Modified libnfc library allowing concurrent scans from multiple NFC readers. (using fork and Unix pipes).
2. Node.js script on server side handling the readers output and the real-time communication with the back-end server. 
3. Node.js back-end server for HTTP requests, database access and events handling (push notification on RFID scan). 
4. Front-end written in Angular and Socket.io.


## How to use

==============

RFID Linux Installation Procedure

===============

17 Jun 2014 

(C) Rebar Design

====

==1==

Copy /voltLib files from USB

run as root :

chmod +x installRFID

./installRFID


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

chmod +x rfid-reader




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

Create /etc/init.d/readerStart 

Write:

- #!/bin/bash
- [Replace with path of readData folder]
- cd /home/voltRebar/readData/
- [Pinpoint node path]
- /usr/local/bin/node index.js &

Save and Close

chmod +x readerStart

====

==6== 

reboot

Scan chargers 
Results will appear in console

## Contact

 [@SebFlorian](https://twitter.com/SebFlorian).
