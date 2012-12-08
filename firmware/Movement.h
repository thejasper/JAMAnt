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
#include "Leg.h"
#include <math.h>

#ifndef MOVEMENT_H
#define MOVEMENT_H

enum move_turns {TURN_CCW = 1, TURN_CW = -1};

void move_calibrate();
void move_calibrate2();
void move_updown();
void move_updownall();
void move_test();
void move_collapse();
void move_expand();
void move_default();
void move_walk(int walk_speed, int scale, boolean drop);
void move_turn(int turn_dir, int angle, int turn_speed);
void move_circle();

#endif
