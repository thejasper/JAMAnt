/*

    Firmware Stigmergic Ant
    Copyright (C) 2012 Cesar Vandevelde <cesar.vandevelde@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "WProgram.h"
#include <math.h>

#ifndef LEG_H
#define LEG_H

enum Leg_indices {LF = 0, LM = 1, LB = 2, RF = 3, RM = 4, RB = 5};
enum Leg_sides {LEG_LEFT = 1, LEG_RIGHT = -1};

// Lengths of segments
// L1 is horizontal distance
#define LEG_L1 50
#define LEG_L2 70
#define LEG_L3 128

// Vertical distance between origin and axis of rotation of servo 2
#define LEG_Z1 -50

// Microseconden pulsewidth per radial.
// 180° * 10µs/° / pi = 572.957...
#define MICROS_PER_RAD 573

class Leg
{
public:
    Leg(int pin1, int pin2, int pin3);

    // Setting leg properties
    void setZeroPositions(int pos1, int pos2, int pos3);
    void setSide(int side);
    void setPosition(int x, int y);

    // Mathematical functions
    void globalToLocalCoords(long g_x, long g_y, long g_z, long *l_x, long *l_y, long *l_z);
    void calcLeg(long x, long y, long z, float *alpha, float *beta, float *gamma);

    // Movement functions
    void moveToGlobalCoords(long x, long y, long z, int duration, boolean start);
    void sendRadians(float alpha, float beta, float gamma, int duration, boolean start);
    void sendDegrees(int deg1, int deg2, int deg3, int duration, boolean start);
    void sendPulses(int pos1, int pos2, int pos3, int duration, boolean start);

private:
    int _pins[3];
    int _zeros[3];
    int _side;
    int _x;
    int _y;
};

#endif
