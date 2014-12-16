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

#ifndef ledstripe_h
#define ledstripe_h

#ifndef REDLEDPIN
#define REDLEDPIN 5
#endif

#ifndef GREENLEDPIN
#define GREENLEDPIN 6
#endif

#ifndef BLUELEDPIN
#define BLUELEDPIN 3
#endif

#include <Arduino.h>

class LedStripe {
    public:
        LedStripe();
        void setRed(byte red);
        void setGreen(byte green);
        void setBlue(byte blue);
        void setColors(byte red, byte green, byte blue);
        void writeColors();
        void idleMode();
        void glowingMode();
        void setMode(byte newMode[4]);
        void doCurrentMode();

    private:
        byte red;
        byte green;
        byte blue;
        byte mode;
        bool glowing_red_ascending;
        bool glowing_green_ascending;
        bool glowing_blue_ascending;
        byte glowing_active_color;
};

#endif
