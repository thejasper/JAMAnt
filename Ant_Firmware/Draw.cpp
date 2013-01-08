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
 
#include "Draw.h"

extern Leg legs[6];

void Draw::convert_coordinates(int screen_x, int screen_y, int* x, int* y)
{
  *x = (*x * max_x) / (screen_x / 2) - max_x;
  *y = (*y * max_y) / (screen_y / 2) - max_y;
}

void Draw::deviate_from_default_pose(int x, int y, int z)
{
  // x en y tussen [-30..30], z bepaalt de hoogte
  //int z = draw ? draw_height : move_height;
  
  // tussen 0 en 84 als er tussen [-30..30] gewerkt wordt
  double dist = sqrt(sq(x-prev_x)+sq(y-prev_y)); 
  
  // hoe groter de afstand hoe langer het mag duren
  int duration = dist * 20;
  
  legs[LF].moveToGlobalCoords(pose[LF][0] - x,
                              pose[LF][1] - y,
                              pose_height - z,
                              duration,
                              false);
  legs[LM].moveToGlobalCoords(pose[LM][0] - x,
                              pose[LM][1] - y,
                              pose_height - z,
                              duration,
                              false);
  legs[LB].moveToGlobalCoords(pose[LB][0] - x,
                              pose[LB][1] - y,
                              pose_height - z,
                              duration,
                              false);
  legs[RF].moveToGlobalCoords(pose[RF][0] - x,
                              pose[RF][1] - y,
                              pose_height - z,
                              duration,
                              false);
  legs[RM].moveToGlobalCoords(pose[RM][0] - x,
                              pose[RM][1] - y,
                              pose_height - z,
                              duration,
                              false);
  legs[RB].moveToGlobalCoords(pose[RB][0] - x,
                              pose[RB][1] - y,
                              pose_height - z,
                              duration,
                              true);               
  prev_x = x;
  prev_y = y;
}
