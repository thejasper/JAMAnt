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

#include "Arduino.h"
#include "Leg.h"
#include <math.h>

#ifndef DRAW_H
#define DRAW_H

const int pose[6][2] = {
    {154, 169},
    {0, 228},
    { -154, 169},
    {154, -169},
    {0, -228},
    { -154, -169}
};
const int pose_height = -130;
  
class Draw
{
public:
  Draw() : max_x(30), max_y(30), prev_x(0), prev_y(0) {}
  
  void convert_coordinates(int screen_x, int screen_y, int* x, int* y);
  void deviate_from_default_pose(int x, int y, int z);
//  void set_draw_height(int height) { draw_height = height; set_move_height(height + 10); }
//  void set_move_height(int height) { move_height = height; }
  
private:
  int prev_x, prev_y;
  int max_x, max_y;
  //int draw_height, move_height;
};

#endif
