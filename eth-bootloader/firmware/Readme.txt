Name: Readme.txt
Author:
Copyright: Arduino
License: GPL http://www.gnu.org/licenses/gpl-2.0.html
Project: eboot
Function: Project documentation
Version: 0.1 tftp / flashing functional

Ethernet bootloader for the ATmega328P / W5100

Flashing bootloader:
--------------------
Connect an AVR ISP MkII and "make install".

Configuring your network:
-------------------------
The bootloader default address is 192.168.1.250.
Configure your computer network card to a static address of 192.168.1.1 with a subnet of 255.255.255.0.
By default, the bootloader assumes an internet gateway is at address 192.168.1.1.

Converting firmware to the right format:
----------------------------------------
The bootloader accepts raw binary images, starting at address 0x0000.
These can be generated using avr-objcopy, part of WinAVR / AVR-GCC, using the "-O binary" option.
Example: avr-objcopy -j .text -j .data -O binary app.elf app.bin

Uploading firmware manually:
----------------------------
1) Check the target board is powered, and connected to the computer ethernet.
2) Verify the computer network settings: Static IP of 192.168.1.1, Subnet of 255.255.255.0.
3) Push reset button to start the bootloader. The LED will blink rapidly.
4) In a console window: tftp -e 192.168.1.250
5) At the tftp> prompt: put app.bin (where app.bin is your application binary image)
6) The board will be reprogrammed. Type quit to exit tftp.

Flash codes:
------------
Rapid blinking: Ethernet bootloader is running.
Slow fading: Test application is running.


Version history
---------------
0.1: First internal release. Supports uploads on tftp.

