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
 *    Authors: Stefan Luecke <glaxx@glaxx.net>
 */
#include "beacon.h"

Beacon::Beacon(void) {
    this->mode = 0;
}

void Beacon::setMode(byte newMode[4]) {
    this->mode = newMode[0]; //TODO
}

void Beacon::doCurrentMode() {
    switch(this->mode) {
        case 65:   digitalWrite(BEACONPIN, HIGH);
                    break;
        case 66:   digitalWrite(BEACONPIN, LOW);
                    break;
    }
}

