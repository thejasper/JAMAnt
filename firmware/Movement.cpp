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

#include "Movement.h"

extern Leg legs[6];

// Default pose
// First index is leg index (e.g. LF)
// Second index is X, Y
const int pose[6][2] =
{
    {154, 169},
    {0, 228},
    { -154, 169},
    {154, -169},
    {0, -228},
    { -154, -169}
};
const int poseheight = -130;

// Offset table for walk cycle
// X and Z offsets
const int walkcycle[16][2] =
{
    {0, 0},
    { -13, 0},
    { -25, 0},
    { -38, 0},
    { -50, 0},
    { -43, 14},
    { -30, 23},
    { -16, 28},
    {0, 30},
    {16, 28},
    {30, 23},
    {43, 14},
    {50, 0},
    {38, 0},
    {25, 0},
    {13, 0}
};

// Z offset for turn cycle
const int turnheight[16] =
{
    0,
    0,
    0,
    0,
    0,
    14,
    23,
    28,
    30,
    28,
    23,
    14,
    0,
    0,
    0,
    0
};
// Angle factors for turn cycle, -1 --> 1
const float turnangle[16] =
{
    0.0f,
    -0.25f,
    -0.5f,
    -0.75f,
    -1.0f,
    -0.865f,
    -0.608f,
    -0.3122f,
    0.0f,
    0.3122f,
    0.608f,
    0.865f,
    1.0f,
    0.75f,
    0.5f,
    0.25f
};

const int circle[30][2] =
{
    {8, 0},
    {16, 0},
    {25, 0},
    {30, 4},
    {27, 12},
    {23, 19},
    {17, 25},
    {10, 28},
    {2, 30},
    { -6, 29},
    { -14, 27},
    { -21, 22},
    { -26, 16},
    { -29, 8},
    { -30, 0},
    { -29, -8},
    { -26, -16},
    { -21, -22},
    { -14, -27},
    { -6, -29},
    {2, -30},
    {10, -28},
    {17, -25},
    {23, -19},
    {27, -12},
    {30, -4},
    {25, 0},
    {16, 0},
    {8, 0},
    {0, 0}
};


void move_calibrate()
{
    legs[LF].sendDegrees(0, 0, 90, 200, false);
    legs[LM].sendDegrees(0, 0, 90, 200, false);
    legs[LB].sendDegrees(0, 0, 90, 200, false);
    legs[RF].sendDegrees(0, 0, 90, 200, false);
    legs[RM].sendDegrees(0, 0, 90, 200, false);
    legs[RB].sendDegrees(0, 0, 90, 200, true);
}

void move_calibrate2()
{
    legs[LF].sendDegrees(0, -90, 90, 200, false);
    legs[LM].sendDegrees(0, -90, 90, 200, false);
    legs[LB].sendDegrees(0, -90, 90, 200, false);
    legs[RF].sendDegrees(0, -90, 90, 200, false);
    legs[RM].sendDegrees(0, -90, 90, 200, false);
    legs[RB].sendDegrees(0, -90, 90, 200, true);
}

void move_updown()
{
    int servo = LF;

    Serial.println("Testing servo 1");
    legs[servo].sendDegrees(20, 0, 90, 300, true);
    delay(1000);
    legs[servo].sendDegrees(-20, 0, 90, 300, true);
    delay(1000);
    legs[servo].sendDegrees(0, 0, 90, 300, true);
    delay(1000);

    Serial.println("Testing servo 2");
    legs[servo].sendDegrees(0, 20, 90, 300, true);
    delay(1000);
    legs[servo].sendDegrees(0, -20, 90, 300, true);
    delay(1000);
    legs[servo].sendDegrees(0, 0, 90, 300, true);
    delay(1000);

    Serial.println("Testing servo 3");
    legs[servo].sendDegrees(0, 0, 110, 300, true);
    delay(1000);
    legs[servo].sendDegrees(0, 0, 70, 300, true);
    delay(1000);
    legs[servo].sendDegrees(0, 0, 90, 300, true);
}

void move_updownall()
{
    Serial.println("Resetting all");

    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 90, 2000, false);
    }
    legs[5].sendDegrees(0, 0, 90, 2000, true);
    delay(3000);

    Serial.println("Testing servo 1");
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(20, 0, 90, 300, false);
    }
    legs[5].sendDegrees(20, 0, 90, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(-20, 0, 90, 300, false);
    }
    legs[5].sendDegrees(-20, 0, 90, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 90, 300, false);
    }
    legs[5].sendDegrees(0, 0, 90, 300, true);
    delay(1000);

    Serial.println("Testing servo 2");
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 20, 90, 300, false);
    }
    legs[5].sendDegrees(0, 20, 90, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, -20, 90, 300, false);
    }
    legs[5].sendDegrees(0, -20, 90, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 90, 300, false);
    }
    legs[5].sendDegrees(0, 0, 90, 300, true);
    delay(1000);

    Serial.println("Testing servo 3");
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 110, 300, false);
    }
    legs[5].sendDegrees(0, 0, 110, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 70, 300, false);
    }
    legs[5].sendDegrees(0, 0, 70, 300, true);
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
        legs[i].sendDegrees(0, 0, 90, 300, false);
    }
    legs[5].sendDegrees(0, 0, 90, 300, true);
}

void move_test()
{
    legs[LF].moveToGlobalCoords(154, 169, -130, 300, false);
    legs[LM].moveToGlobalCoords(0, 228, -130, 300, false);
    legs[LB].moveToGlobalCoords(-154, 169, -130, 300, false);
    legs[RF].moveToGlobalCoords(154, -169, -130, 300, false);
    legs[RM].moveToGlobalCoords(0, -228, -130, 300, false);
    legs[RB].moveToGlobalCoords(-154, -169, -130, 300, true);
}

void move_collapse()
{
    // Reset head
    Serial2.print("#4 P1000 #5 P1510 T200\r");

    // LF to 0° z-rotation
    legs[LF].moveToGlobalCoords(pose[LF][0], pose[LF][1], poseheight + 20, 50, true);
    delay(50);
    legs[LF].moveToGlobalCoords(90, 58 + 170, poseheight + 20, 100, true);
    delay(100);
    legs[LF].moveToGlobalCoords(90, 58 + 170, poseheight, 50, true);
    delay(50);

    // RF to 0° z-rotation
    legs[RF].moveToGlobalCoords(pose[RF][0], pose[RF][1], poseheight + 20, 50, true);
    delay(50);
    legs[RF].moveToGlobalCoords(90, -58 - 170, poseheight + 20, 100, true);
    delay(100);
    legs[RF].moveToGlobalCoords(90, -58 - 170, poseheight, 50, true);
    delay(50);

    // LB to 0° z-rotation
    legs[LB].moveToGlobalCoords(pose[LB][0], pose[LB][1], poseheight + 20, 50, true);
    delay(50);
    legs[LB].moveToGlobalCoords(-90, 58 + 170, poseheight + 20, 100, true);
    delay(100);
    legs[LB].moveToGlobalCoords(-90, 58 + 170, poseheight, 50, true);
    delay(50);

    // RB to 0° z-rotation
    legs[RB].moveToGlobalCoords(pose[RB][0], pose[RB][1], poseheight + 20, 50, true);
    delay(50);
    legs[RB].moveToGlobalCoords(-90, -58 - 170, poseheight + 20, 100, true);
    delay(100);
    legs[RB].moveToGlobalCoords(-90, -58 - 170, poseheight, 50, true);
    delay(50);

    // Move all legs up
    int currentheight;
    for (currentheight = poseheight; currentheight < -60; currentheight += 10)
    {
        legs[LF].moveToGlobalCoords(90, 58 + 170, currentheight, 50, false);
        legs[LM].moveToGlobalCoords(0, 58 + 170, currentheight, 50, false);
        legs[LB].moveToGlobalCoords(-90, 58 + 170, currentheight, 50, false);
        legs[RF].moveToGlobalCoords(90, -58 - 170, currentheight, 50, false);
        legs[RM].moveToGlobalCoords(0, -58 - 170, currentheight, 50, false);
        legs[RB].moveToGlobalCoords(-90, -58 - 170, currentheight, 50, true);
        delay(50);
    }

    // Fold all legs
    legs[LF].sendDegrees(0, -90, 130, 100, false);
    legs[LM].sendDegrees(0, -90, 130, 100, false);
    legs[LB].sendDegrees(0, -90, 130, 100, false);
    legs[RF].sendDegrees(0, -90, 130, 100, false);
    legs[RM].sendDegrees(0, -90, 130, 100, false);
    legs[RB].sendDegrees(0, -90, 130, 100, true);
    delay(100);
}

void move_expand()
{
    // Reset head
    Serial2.print("#4 P1000 #5 P1510 T200\r");

    // Set to default collapsed position
    legs[LF].sendDegrees(0, -90, 130, 100, false);
    legs[LM].sendDegrees(0, -90, 130, 100, false);
    legs[LB].sendDegrees(0, -90, 130, 100, false);
    legs[RF].sendDegrees(0, -90, 130, 100, false);
    legs[RM].sendDegrees(0, -90, 130, 100, false);
    legs[RB].sendDegrees(0, -90, 130, 100, true);
    delay(100);

    // Move all legs down
    int currentheight;
    for (currentheight = -70; currentheight >= poseheight; currentheight -= 10)
    {
        legs[LF].moveToGlobalCoords(90, 58 + 170, currentheight, 50, false);
        legs[LM].moveToGlobalCoords(0, 58 + 170, currentheight, 50, false);
        legs[LB].moveToGlobalCoords(-90, 58 + 170, currentheight, 50, false);
        legs[RF].moveToGlobalCoords(90, -58 - 170, currentheight, 50, false);
        legs[RM].moveToGlobalCoords(0, -58 - 170, currentheight, 50, false);
        legs[RB].moveToGlobalCoords(-90, -58 - 170, currentheight, 50, true);
        delay(50);
    }

    // LF to default pos
    legs[LF].moveToGlobalCoords(90, 58 + 170, poseheight + 20, 50, true);
    delay(50);
    legs[LF].moveToGlobalCoords(pose[LF][0], pose[LF][1], poseheight + 20, 100, true);
    delay(100);
    legs[LF].moveToGlobalCoords(pose[LF][0], pose[LF][1], poseheight, 50, true);
    delay(50);

    // RF to default pos
    legs[RF].moveToGlobalCoords(90, -58 - 170, poseheight + 20, 50, true);
    delay(50);
    legs[RF].moveToGlobalCoords(pose[RF][0], pose[RF][1], poseheight + 20, 100, true);
    delay(100);
    legs[RF].moveToGlobalCoords(pose[RF][0], pose[RF][1], poseheight, 50, true);
    delay(50);

    // LB to default pos
    legs[LB].moveToGlobalCoords(-90, 58 + 170, poseheight + 20, 50, true);
    delay(50);
    legs[LB].moveToGlobalCoords(pose[LB][0], pose[LB][1], poseheight + 20, 100, true);
    delay(100);
    legs[LB].moveToGlobalCoords(pose[LB][0], pose[LB][1], poseheight, 50, true);
    delay(50);

    // RB to default pos
    legs[RB].moveToGlobalCoords(-90, -58 - 170, poseheight + 20, 50, true);
    delay(50);
    legs[RB].moveToGlobalCoords(pose[RB][0], pose[RB][1], poseheight + 20, 100, true);
    delay(100);
    legs[RB].moveToGlobalCoords(pose[RB][0], pose[RB][1], poseheight, 50, true);
    delay(50);
}

void move_default()
{
    // Reset head
    Serial2.print("#4 P1000 #5 P1510 T200\r");

    // Reset legs
    legs[LF].moveToGlobalCoords(pose[LF][0], pose[LF][1], poseheight, 100, false);
    legs[LM].moveToGlobalCoords(pose[LM][0], pose[LM][1], poseheight, 100, false);
    legs[LB].moveToGlobalCoords(pose[LB][0], pose[LB][1], poseheight, 100, false);
    legs[RF].moveToGlobalCoords(pose[RF][0], pose[RF][1], poseheight, 100, false);
    legs[RM].moveToGlobalCoords(pose[RM][0], pose[RM][1], poseheight, 100, false);
    legs[RB].moveToGlobalCoords(pose[RB][0], pose[RB][1], poseheight, 100, true);
}

void move_turn(int turn_dir, int angle, int turn_speed)
{
    // Movement cycle indices
    int tripod1 = 0; // LF, RM, LB
    int tripod2 = 8; // RF, LM, RB

    // Turn head
    if (turn_dir == TURN_CCW)
    {
        Serial2.print("#5 P1210 T500");
    }
    else
    {
        Serial2.print("#5 P1810 T500");
    }

    float _sin, _cos;
    float maxangle = (float)angle * 3.1415926f * 1.15f / (180.0f * 4.0f); // 2 tripods * 2 directions, 1.25 fudge factor

    for (int i = 0; i < 16; i++)
    {
        // Move Tripod 1 legs to their correct positions.
        _sin = sin(maxangle * turnangle[tripod1]);
        _cos = cos(maxangle * turnangle[tripod1]);

        legs[LF].moveToGlobalCoords(pose[LF][0]*_cos - turn_dir * pose[LF][1]*_sin,
                                    turn_dir * pose[LF][0]*_sin + pose[LF][1]*_cos,
                                    poseheight + turnheight[tripod1],
                                    turn_speed,
                                    false);
        legs[RM].moveToGlobalCoords(pose[RM][0]*_cos - turn_dir * pose[RM][1]*_sin,
                                    turn_dir * pose[RM][0]*_sin + pose[RM][1]*_cos,
                                    poseheight + turnheight[tripod1],
                                    turn_speed,
                                    false);
        legs[LB].moveToGlobalCoords(pose[LB][0]*_cos - turn_dir * pose[LB][1]*_sin,
                                    turn_dir * pose[LB][0]*_sin + pose[LB][1]*_cos,
                                    poseheight + turnheight[tripod1],
                                    turn_speed,
                                    false);

        // Move Tripod 2 legs to their correct positions.
        _sin = sin(maxangle * turnangle[tripod2]);
        _cos = cos(maxangle * turnangle[tripod2]);

        legs[RF].moveToGlobalCoords(pose[RF][0]*_cos - turn_dir * pose[RF][1]*_sin,
                                    turn_dir * pose[RF][0]*_sin + pose[RF][1]*_cos,
                                    poseheight + turnheight[tripod2],
                                    turn_speed,
                                    false);
        legs[LM].moveToGlobalCoords(pose[LM][0]*_cos - turn_dir * pose[LM][1]*_sin,
                                    turn_dir * pose[LM][0]*_sin + pose[LM][1]*_cos,
                                    poseheight + turnheight[tripod2],
                                    turn_speed,
                                    false);
        legs[RB].moveToGlobalCoords(pose[RB][0]*_cos - turn_dir * pose[RB][1]*_sin,
                                    turn_dir * pose[RB][0]*_sin + pose[RB][1]*_cos,
                                    poseheight + turnheight[tripod2],
                                    turn_speed,
                                    true);

        tripod1++;
        tripod2++;

        if (tripod1 == 16)
        {
            tripod1 = 0;
        }

        if (tripod2 == 16)
        {
            tripod2 = 0;
        }

        delay(turn_speed);
    }
}

void move_walk(int walk_speed, int scale, boolean drop)
{
    // Movement cycle indices
    int tripod1 = 0; // LF, RM, LB
    int tripod2 = 8; // RF, LM, RB
    static boolean openclose = false;
    float scale_factor = (scale == 10) ? 1.0 : (scale / 10.0);

    // Head straight
    Serial2.print("#5 P1510 T500");

    // Drop M&Ms
    if (drop)
    {
        if (openclose)
        {
            Serial2.print("#24 P1100 T200\r");
        }
        else
        {
            Serial2.print("#24 P1900 T200\r");
        }
        openclose = openclose ? false : true;
    }

    for (int i = 0; i < 16; i++)
    {
        // Jaws
        if (i == 0)
        {
            Serial2.print("#4 P1500 T");
            Serial2.print(walk_speed * 8);
            Serial2.print("\r");
        }
        else if (i == 8)
        {
            Serial2.print("#4 P1000 T");
            Serial2.print(walk_speed * 8);
            Serial2.print("\r");
        }

        // Move Tripod 1 legs to their correct positions.
        legs[LF].moveToGlobalCoords(pose[LF][0] + scale_factor * walkcycle[tripod1][0],
                                    pose[LF][1],
                                    poseheight + walkcycle[tripod1][1],
                                    walk_speed,
                                    false);
        legs[RM].moveToGlobalCoords(pose[RM][0] + scale_factor * walkcycle[tripod1][0],
                                    pose[RM][1],
                                    poseheight + walkcycle[tripod1][1],
                                    walk_speed,
                                    false);
        legs[LB].moveToGlobalCoords(pose[LB][0] + scale_factor * walkcycle[tripod1][0],
                                    pose[LB][1],
                                    poseheight + walkcycle[tripod1][1],
                                    walk_speed,
                                    false);
        // Move Tripod 2 legs to their correct positions.
        legs[RF].moveToGlobalCoords(pose[RF][0] + scale_factor * walkcycle[tripod2][0],
                                    pose[RF][1],
                                    poseheight + walkcycle[tripod2][1],
                                    walk_speed,
                                    false);
        legs[LM].moveToGlobalCoords(pose[LM][0] + scale_factor * walkcycle[tripod2][0],
                                    pose[LM][1],
                                    poseheight + walkcycle[tripod2][1],
                                    walk_speed,
                                    false);
        legs[RB].moveToGlobalCoords(pose[RB][0] + scale_factor * walkcycle[tripod2][0],
                                    pose[RB][1],
                                    poseheight + walkcycle[tripod2][1],
                                    walk_speed,
                                    true);
        tripod1++;
        tripod2++;

        if (tripod1 == 16)
        {
            tripod1 = 0;
        }

        if (tripod2 == 16)
        {
            tripod2 = 0;
        }

        delay(walk_speed);
    }
}

void move_circle()
{
    for (int i = 0; i < 30; i++)
    {
        legs[LF].moveToGlobalCoords(pose[LF][0] - circle[i][0],
                                    pose[LF][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    false);
        legs[LM].moveToGlobalCoords(pose[LM][0] - circle[i][0],
                                    pose[LM][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    false);
        legs[LB].moveToGlobalCoords(pose[LB][0] - circle[i][0],
                                    pose[LB][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    false);
        legs[RF].moveToGlobalCoords(pose[RF][0] - circle[i][0],
                                    pose[RF][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    false);
        legs[RM].moveToGlobalCoords(pose[RM][0] - circle[i][0],
                                    pose[RM][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    false);
        legs[RB].moveToGlobalCoords(pose[RB][0] - circle[i][0],
                                    pose[RB][1] - circle[i][1],
                                    poseheight,
                                    30,
                                    true);
        delay(30);
    }
}
