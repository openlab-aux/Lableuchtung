/*
 *    Copyright (C) 2014 Stefan Luecke 
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Affero General Public License as published
 *    by the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    Authors: 
 *      Stefan Luecke <glaxx@glaxx.net>
 *      Michael Wendland <michael@michiwend.com>
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>   

#include "ledstripe.h"
#include "beacon.h"

#define ENABLE_AUTOMODE 251
#define ENABLE_BEACON   252
#define DISABLE_BEACON  253


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 11, 7, 3);
unsigned int localPort = 1337;      

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

EthernetUDP Udp;

Beacon beac;
LedStripe leds;

void serialEvent() {
/*
    byte serial_buffer[4];
    Serial.readBytes((char *)serial_buffer, 4);

    switch(serial_buffer[0]) {
        case ENABLE_AUTOMODE:
            // TODO enable automode
            break;

        case ENABLE_BEACON:
            beac.enable();
            break;

        case DISABLE_BEACON:
            beac.disable();
            break;

        case 254: 
            break;

        case 255:
            break;

        default:
            leds.fadeTo(
                    serial_buffer[0],
                    serial_buffer[1],
                    serial_buffer[2],
                    serial_buffer[3]);
            break;
    }

    byte chk = serial_buffer[0] 
             ^ serial_buffer[1] 
             ^ serial_buffer[2] 
             ^ serial_buffer[3];

    Serial.write(chk);
 */
}

void UDPEvent() {

    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if(packetSize == 4) {
   
        Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);

        switch(packetBuffer[0]) {
        case ENABLE_AUTOMODE:
            // TODO enable automode
            break;

        case ENABLE_BEACON:
            beac.enable();
            break;

        case DISABLE_BEACON:
            beac.disable();
            break;

        case 254: 
            break;

        case 255:
            break;

        default:
            leds.fadeTo(
                    packetBuffer[0],
                    packetBuffer[1],
                    packetBuffer[2],
                    packetBuffer[3]);
            break;
        }

        byte chk = packetBuffer[0] 
                 ^ packetBuffer[1] 
                 ^ packetBuffer[2] 
                 ^ packetBuffer[3];

        // send a reply, to the IP address and port that sent us the packet we received
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(chk);
        Udp.endPacket();
    }
}


void setup() {
    pinMode(REDLEDPIN,   OUTPUT);
    pinMode(GREENLEDPIN, OUTPUT);
    pinMode(BLUELEDPIN,  OUTPUT);
    pinMode(BEACONPIN,   OUTPUT);
    Serial.begin(115200);

    // start the Ethernet and UDP:
    Ethernet.begin(mac, ip);
    Udp.begin(localPort);
}


void loop() {
    UDPEvent();
}


