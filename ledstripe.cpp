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

#include "ledstripe.h"

LedStripe::LedStripe(void) {
    this->red   = 0;
    this->green = 0;
    this->blue  = 0;
    this->mode  = 0;
    this->glowing_red_ascending   = true;
    this->glowing_green_ascending = true;
    this->glowing_blue_ascending  = true;
    this->glowing_active_color    = 0;
}

void LedStripe::setColors(byte red, byte green, byte blue) {
    this->red   = red;
    this->green = green;
    this->blue  = blue;
}

void LedStripe::writeColors() {
    analogWrite(REDLEDPIN,   this->red);
    analogWrite(GREENLEDPIN, this->green);
    analogWrite(BLUELEDPIN,  this->blue);
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


void LedStripe::fadeTo(byte t, byte r, byte g, byte b) {

    if(t == 0) {
        this->setColors(r, g, b);
        this->writeColors();
        return;
    }

    int dR = r - this->red;
    int dG = g - this->green;
    int dB = b - this->blue;

    int maxDelta = max(abs(dR) ,max(abs(dG), abs(dB)));
    if(maxDelta == 0) {
        return;
    }

     // Float colors (for calculation)
    float fR = this->red;
    float fG = this->green;
    float fB = this->blue;

    // Step for each chan
    float sR = (float)dR / (float)maxDelta;
    float sG = (float)dG / (float)maxDelta;
    float sB = (float)dB / (float)maxDelta;

    long d = ((float)t / (float)maxDelta) * 100.0 * 1000.0;

    for(int i = 0; i < maxDelta; i++) {
        fR += sR;
        fG += sG;
        fB += sB;

        this->setColors((int)fR, (int)fG, (int)fB);
        this->writeColors();

        // see http://arduino.cc/en/Reference/DelayMicroseconds
        if(d > 16383) {
            delay(d/1000);
        }
        else {
            delayMicroseconds(d);
        }
    }
    this->setColors(r, g, b);
    this->writeColors();
}
