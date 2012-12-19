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

#include "Leg.h"

Leg::Leg(int pin1, int pin2, int pin3)
{
  _pins[0] = pin1;
  _pins[1] = pin2;
  _pins[2] = pin3;
}

void Leg::setZeroPositions(int pos1, int pos2, int pos3)
{
  _zeros[0] = pos1;
  _zeros[1] = pos2;
  _zeros[2] = pos3;
}

void Leg::setSide(int side)
{
  _side = side;
}

void Leg::setPosition(int x, int y)
{
  // X: center to head
  // Y: center to left side

  _x = x;
  _y = y;
}

void Leg::globalToLocalCoords(long g_x, long g_y, long g_z, long *l_x, long *l_y, long *l_z)
{
  // Global coordinate system:
  //   X: center to head
  //   Y: center to left side
  //   Z: up
  // Local coordinate system:
  //   X: points to leg when the leg is in zero position
  //   Y: points to back
  //   Z: up
  *l_x = (g_y - _y) * _side;
  *l_y = -g_x + _x;
  *l_z = g_z;
}

void Leg::calcLeg(long x, long y, long z, float *alpha, float *beta, float *gamma)
{
  // Takes LOCAL coordinates and returns 3 rotation angles in RADIANS
  // Z: up
  // X: points to leg when the leg is in zero position
  // Y: points to back

  float dz, r, d, a, x2, y2, h, rx, ry, xi, yi;

  *alpha = atan2(y, x);

  dz = z - LEG_Z1;
  r = sqrt(x * x + y * y) - LEG_L1;

  d = sqrt(r * r + dz * dz);
  a = (LEG_L2 * LEG_L2 - LEG_L3 * LEG_L3 + d * d) / (2 * d);
  x2 = r * a / d;
  y2 = dz * a / d;
  h = sqrt(LEG_L2 * LEG_L2 - a * a);
  rx = -dz * h / d;
  ry = r * h / d;
  xi = x2 + rx;
  yi = y2 + ry;

  *beta = -atan2(yi, xi);
  *gamma = atan2(yi - dz, r - xi) - *beta;
}

void Leg::moveToGlobalCoords(long x, long y, long z, int duration, boolean start)
{
  long l_x, l_y, l_z;
  float alpha, beta, gamma;

  globalToLocalCoords(x, y, z, &l_x, &l_y, &l_z);
  calcLeg(l_x, l_y, l_z, &alpha, &beta, &gamma);
  sendRadians(alpha, beta, gamma, duration, start);
}

void Leg::sendDegrees(int deg1, int deg2, int deg3, int duration, boolean start)
{
  // +deg1 = rotating to back
  // +deg2 = rotating down
  // +deg3 = rotating down
  sendPulses(_zeros[0] + deg1 * 10 * _side,
  _zeros[1] + deg2 * 10 * _side,
  _zeros[2] - deg3 * 10 * _side,
  duration,
  start);
}

void Leg::sendRadians(float alpha, float beta, float gamma, int duration, boolean start)
{
  // +alpha = rotating to back
  // +beta = rotating down
  // +gamma = rotating down
  sendPulses(_zeros[0] + alpha * MICROS_PER_RAD * _side,
  _zeros[1] + beta * MICROS_PER_RAD * _side,
  _zeros[2] - gamma * MICROS_PER_RAD * _side,
  duration,
  start);
}

void Leg::sendPulses(int p1, int p2, int p3, int duration, boolean start)
{
  // Ignores duration if start == false;

  p1 = constrain(p1, 850, 2150);
  p2 = constrain(p2, 850, 2150);
  p3 = constrain(p3, 850, 2150);

  Serial2.print("#");
  Serial2.print(_pins[0]);
  Serial2.print(" P");
  Serial2.print(p1);
  Serial2.print(" #");
  Serial2.print(_pins[1]);
  Serial2.print(" P");
  Serial2.print(p2);
  Serial2.print(" #");
  Serial2.print(_pins[2]);
  Serial2.print(" P");
  Serial2.print(p3);
  if (start)
  {
    Serial2.print(" T");
    Serial2.print(duration);
    Serial2.print("\r");
  }
  else
  {
    Serial2.print(" ");
  }
}

