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
boolean drop_MnM = true;

// Define legs with correct pins
Leg legs[6] = {
  Leg(12, 13, 14),  //LF
  Leg(8, 9, 10),    //LM
  Leg(28, 29, 30),  //LB
  
  Leg(0, 1, 2),     //RF
  Leg(20, 21, 22),  //RM
  Leg(16, 17, 18)   //RB
};

void setup(){
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
  
  //Serial.println("Connected");
  Serial.println("CND"); // Connected
}

void loop(){
  process_communication();
}

void process_communication(){
  if(Serial.available() > 0){
    inc_data = Serial.read();
    
    if(inc_data == '\n'){
      if(buffer == "PNG"){
        // Ping
        Serial.println("PNG OK");
      }else if(buffer == "EXP"){
        // Expand
        move_expand();
        Serial.println("EXP OK");
      }else if(buffer == "CLP"){
        // Collapse
        move_default();
        move_collapse();
        Serial.println("CLP OK");
      }else if(buffer == "DRP"){
        // Drop
        Serial2.print("#24 P1100 T200\r");
        delay(1000);
        Serial2.print("#24 P1900 T200\r");
        delay(500);
        Serial.println("DRP OK");
      }else if(buffer == "DFT"){
        // Default pose
        move_default();
        Serial.println("DFT OK");
      }else if(buffer == "WLK"){
        // Walk
        move_walk(ant_speed, 10, drop_MnM);
        Serial.println("WLK OK");
      }else if(buffer == "THD"){
        // Test head
        Serial2.print("#5 P1510 #4 P1000 T100\r"); // Center head
        delay(100);
        Serial2.print("#5 P1210 T300\r");
        delay(1000);
        Serial2.print("#5 P1810 T600\r");
        delay(1300);
        Serial2.print("#5 P1510 T300\r");
        delay(1000);
        Serial2.print("#4 P1500 T400\r");
        delay(400);
        Serial2.print("#4 P1000 T400\r");
        delay(400);
        Serial2.print("#4 P1500 T400\r");
        delay(400);
        Serial2.print("#4 P1000 T400\r");
        delay(400);
      }else if(buffer == "CRL"){
        move_default();
        move_circle();
      }else if(buffer.startsWith("TRN ")){
        // Turn
        int angle = StringToInt(buffer.substring(4));
        int dir = angle > 0 ? TURN_CCW : TURN_CW;
        angle = constrain(abs(angle), 0, 45);
        move_turn(dir, angle, ant_speed);
        Serial.println("TRN OK");
      }else if(buffer.startsWith("WLK ")){
        // Walk
        int scale = StringToInt(buffer.substring(4));
        scale = constrain(scale, 0, 10);
        move_walk(ant_speed, scale, drop_MnM);
        Serial.println("WLK OK");
      }else if(buffer.startsWith("SPD ")){
        // Speed
        ant_speed = StringToInt(buffer.substring(4));
        Serial.println("SPD OK");
      }else if(buffer.startsWith("CLB ")){
        // Calibrate
        char pose = buffer.charAt(4);
        if(pose == 'A'){
          move_calibrate();
          Serial.println("CLB OK");
        }else if(pose == 'B'){
          move_calibrate2();
          Serial.println("CLB OK");
        }
      }else if(buffer.startsWith("DRP ")){
        int drop = StringToInt(buffer.substring(4));
        if(drop == 1){
          drop_MnM = true;
          Serial.println("DRP OK");
        }else if(drop == 0){
          drop_MnM = false;
          Serial.println("DRP OK");
        }
      }else if(buffer.startsWith("JAW ")){
        int pos = StringToInt(buffer.substring(4));
        pos = constrain(pos, 1000, 1800);
        Serial2.print("#4 P");
        Serial2.print(pos);
        Serial2.print(" T200\r");
      }else if(buffer == "CBT"){
        // Calibrate turn;
        move_turn(TURN_CCW, 45, ant_speed);
        move_turn(TURN_CCW, 45, ant_speed);
        move_default();
      }
      
      buffer = "";
    }else{
      buffer += inc_data;
    }
  }
}

int StringToInt(String str){
  int len = str.length() + 1;
  char* str2 = (char*)malloc(len); // Length is without \0
  
  str.toCharArray(str2, len);
  int result = atoi(str2);
  
  free((void*)str2);
  return result;
}
