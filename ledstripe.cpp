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

#include "ledstripe.h"

LedStripe::LedStripe(void) 
{
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->mode = 0;
    this->glowing_red_ascending = true;
    this->glowing_green_ascending = true;
    this->glowing_blue_ascending = true;
    this->glowing_active_color = 0;
}

void LedStripe::setRed(byte red) {
    this->red = red;
}

void LedStripe::setGreen(byte green) {
    this->green = green;
}

void LedStripe::setBlue(byte blue) {
    this->blue = blue;
}

void LedStripe::setColors(byte red, byte green, byte blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

void LedStripe::writeColors() {
    analogWrite(REDLEDPIN, this->red);
    analogWrite(GREENLEDPIN, this->green);
    analogWrite(BLUELEDPIN, this->blue);
}

void LedStripe::idleMode() {
    writeColors();
}

void LedStripe::glowingMode() {
    if (glowing_active_color == 0) {
        if (this->glowing_red_ascending) {
            (this->red == 255) ? this->glowing_red_ascending = false : this->red++;
        } else {
            if (this->red == 0 ) {
               this->glowing_red_ascending = true;
               this->glowing_active_color = 1;
            } else {
                this->red--;
            }
        }
    } else if (glowing_active_color == 1) {
        if (this->glowing_green_ascending) {
            (this->green == 255) ? this->glowing_green_ascending = false : this->green++;
        } else {
            if (this->green == 0) {
                this->glowing_green_ascending = true;
                this->glowing_active_color = 2;
            } else {
                this->green--;
            }
        }
    } else if (glowing_active_color == 2) {
        if (this->glowing_blue_ascending) {
            (this->blue == 255) ? this->glowing_blue_ascending = false : this->blue++;
        } else {
            if (this->blue == 0) {
                this->glowing_blue_ascending = true;
                this->glowing_active_color = 0;
            } else {
                this->blue--;
            }
        }
    } else {
        this->glowing_active_color = 0;
    }
    writeColors();
}

void LedStripe::setMode(byte newMode[4]) {
    Serial.println("setmode ledstripe");
    Serial.println(newMode[0]);
    switch (newMode[0]) {
        case 32:  this->mode = 0;
                    break;
        case 33:  this->mode = 1;
                    break;
        default:    this->mode = 0;
                    break;
    }
}

void LedStripe::doCurrentMode() {
    switch (this->mode) {
        case 0:     idleMode();
                    break;
        case 1:     glowingMode();
                    break;
        default:    idleMode();
                    break;
    }
}

