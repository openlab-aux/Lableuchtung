Lableuchtung
============

![arduino with shield](misc/arduino.jpg)


## WIE LEUCHTEST DU!?
a simple binary leucht-protocol

Every package consists of 4 bytes as follows:
The first byte is the command or time byte. Use 251-255 to invoke
different commands otherwise it is interpreted as time in 10ms. See table

    |commands / time in 10ms |R        |G        |B        |Result
    +------------------------+---------+---------+---------+-----------------
    |11111011   251          |00000000 |00000000 |00000000 |Enable automode
    |11111100   252          |00000000 |00000000 |00000000 |Enable beacon
    |11111101   253          |00000000 |00000000 |00000000 |Disable beacon
    |11111110   254          |00000000 |00000000 |00000000 |
    |11111111   255          |00000000 |00000000 |00000000 |
    +------------------------+---------+---------+---------+-----------------
    |00000000     0          |00000000 |00000000 |00000000 |turn all LEDs off
    |00000000     0          |11111111 |11111111 |11111111 |flash to full white
    |01100100   100          |11111111 |00000000 |00000000 |fade to full red in 1 sec

The response is just the XOR-result over the four bytes the arduino has received.


## Dependencys
  * [arduino-mk](https://github.com/sudar/Arduino-Makefile)

