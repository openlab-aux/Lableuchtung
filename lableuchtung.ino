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
 *    Authors:  Stefan Luecke <glaxx@glaxx.net>
 *              Michael Wendland <michael@michiwend.com>
 */
#include "ledstripe.h"
#include "beacon.h"

Beacon beac;
LedStripe leds;

/*
 * WIE LEUCHTEST DU!?
 *     a simple binary leucht-protocol
 *
 * you can send 1 or 4 bytes as follows:
 * the first byte is the command or time byte. Use 251-255 to invoke
 * different commands otherwise it is interpreted as time in 10ms. See table
 *
 * |commands / time in 10ms |R        |G        |B        |Result
 * +------------------------+---------+---------+---------+-----------------
 * |11111011   251          |         |         |         |Enable automode
 * |11111100   252          |         |         |         |Enable beacon
 * |11111101   253          |         |         |         |Disable beacon
 * |11111110   254          |         |         |         |
 * |11111111   255          |         |         |         |
 * +------------------------+---------+---------+---------+-----------------
 * |00000000     0          |00000000 |00000000 |00000000 |turn all LEDs off
 * |00000000     0          |11111111 |11111111 |11111111 |flash to full white
 * |01100100   100          |11111111 |00000000 |00000000 |fade to full red in 1 sec
 *
 */

void serialEvent() {
    byte index = 0;
    byte serial_buffer [4] = {0, 0, 0, 0};
    while (Serial.available() > 0 && index < 4) {
        serial_buffer[index] = Serial.read();
        index++;
        delay(5); // Needed to read all variables
    }
    Serial.println(serial_buffer[0]);
    Serial.println(serial_buffer[1]);
    Serial.println(serial_buffer[2]);
    Serial.println(serial_buffer[3]);
    if (index > 0) {
        (serial_buffer[0] > 64) ? beac.setMode(serial_buffer) : leds.setMode(serial_buffer);
    }
}

void setup() {
    pinMode(REDLEDPIN, OUTPUT);
    pinMode(GREENLEDPIN, OUTPUT);
    pinMode(BLUELEDPIN, OUTPUT);
    pinMode(BEACONPIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    beac.doCurrentMode();
    leds.doCurrentMode();
    delay(50);
}

