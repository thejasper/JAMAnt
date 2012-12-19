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
#include "Movement.h"

char inc_data = 0;
String buffer = "";
int ant_speed = 20;
boolean drop_MnM = false;

enum ant_action { WALK, TURN, PNG, DRAW, STOP, NOTHING };
  
unsigned int timeout = 2 * 1000;     // langste tijd dat er tussen 2 commando's mag zitten
boolean in_time = false;             // geeft aan of er een timeout is of niet
ant_action current_action = NOTHING; // huidige actie (zie declaratie enum)
int command_parameter = 0;           // extra informatie voor de huidige actie
unsigned long last_ping = 0;         // laatste ping in milliseconden
String coordinates = "";             // coordinaten voor de tekenopdracht

// Define legs with correct pins
Leg legs[6] =
{
  Leg(12, 13, 14),  //LF
  Leg(8, 9, 10),    //LM
  Leg(28, 29, 30),  //LB

  Leg(0, 1, 2),     //RF
  Leg(20, 21, 22),  //RM
  Leg(16, 17, 18)   //RB
};

void setup()
{
  Serial.begin(115200); // Arduino <-> PC
  Serial2.begin(115200); // Arduino <-> SSC-32

  legs[LF].setZeroPositions(1570, 1850, 2370);
  legs[LF].setSide(LEG_LEFT);
  legs[LF].setPosition(90, 58);

  legs[LM].setZeroPositions(1510, 1790, 2430);
  legs[LM].setSide(LEG_LEFT);
  legs[LM].setPosition(0, 58);

  legs[LB].setZeroPositions(1490, 1830, 2430);
  legs[LB].setSide(LEG_LEFT);
  legs[LB].setPosition(-90, 58);


  legs[RF].setZeroPositions(1490, 1280, 680);
  legs[RF].setSide(LEG_RIGHT);
  legs[RF].setPosition(90, -58);

  legs[RM].setZeroPositions(1510, 1170, 660);
  legs[RM].setSide(LEG_RIGHT);
  legs[RM].setPosition(0, -58);

  legs[RB].setZeroPositions(1430, 1250, 600);
  legs[RB].setSide(LEG_RIGHT);
  legs[RB].setPosition(-90, -58);

  Serial.println("CND"); // Connected
}

void loop()
{
  process_communication();
}

void process_communication()
{
  boolean in_time = millis() - last_ping < timeout;
  
  // commando inlezen
  if (Serial.available() > 0)
  {
    inc_data = Serial.read();
    
    if (inc_data == '\n') 
    {
      interpret_buffer();
      buffer = "";
    }
    else
      buffer += inc_data;
  }
  
  // commando uitvoeren 
  execute_action();
}

void interpret_buffer()
{
  if (!in_time)
    current_action = NOTHING;
    
  last_ping = millis();
   
  if (buffer == "PNG") // ping
  {
    // ieder commando telt als ping :)
    Serial.println("PNG OK");
  }
  else if (buffer == "STP") // stop
  {
    current_action = STOP;
    Serial.println("STP OK");
  }
  else if (buffer.startsWith("WLK ")) // walk
  {
    current_action = WALK;
    command_parameter = constrain(buffer.substring(4).toInt(), 0, 10);
    Serial.println("WLK OK");
  }
  else if (buffer.startsWith("TRN ")) // turn
  {
    current_action = TURN;
    command_parameter = constrain(buffer.substring(4).toInt(), -45, 45);
    Serial.println("TRN OK");
  }
  else if (buffer.startsWith("DRW ")) // draw
  {
    current_action = DRAW;
    coordinates = buffer.substring(4);
    Serial.println("DRW OK");
  }
  else // something else
  {
    current_action = NOTHING;
    Serial.println("JNK OK");
  }
}

void execute_action()
{
  if (current_action == STOP) 
  {
    move_default();
    current_action = NOTHING;
    Serial.println("ACTUAL STP OK");
  }
  else if (current_action == WALK && in_time) 
  {
    move_walk(ant_speed, command_parameter);
    Serial.println("ACTUAL WLK OK");
  }
  else if (current_action == TURN && in_time) 
  {
    int dir = command_parameter > 0 ? TURN_CCW : TURN_CW;
    move_turn(dir, abs(command_parameter), ant_speed);
    Serial.println("ACTUAL TRN OK");
  }
  else if (current_action == DRAW)
  {
    // not implemented
    current_action = NOTHING;
    Serial.println("ACTUAL DRW OK");
  }
}

