/**
 * @file main.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Automated Dice Shaker
 * @version 1.0
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdint.h>
#include <Arduino.h>
#include <MD_TCS230.h>
#include <Encoder.h>

#include "config.h"
#include "tcsFunctions.h"
#include "basicFunctions.h"
#include "menuFunctions.h"
#include "gameStateFunctions.h"


void setup(){

  initArduino();
  resetFSM(&fsm, menus);

}

void loop(){

  if(encoderInterrupt){
    (&fsm)->stateTransition = true;
    resetEncoder();
    delay(200);
    encoderInterrupt = false;
  }

  if(resetInterrupt){
    resetFSM(&fsm, menus);
    resetEncoder();
    delay(200);
    resetInterrupt = false;
  }

  gameStateFSM(&fsm, menus);


  /*Serial.print(mainMenuProperties.cursorPos);
  Serial.print("\t");
  Serial.print(mainMenuProperties.topIndex);
  Serial.print("\t");
  Serial.print(mainMenuProperties.itemNumber);
  Serial.print("\t");
  Serial.print(mainMenuProperties.selectedIndex);
  Serial.print("\t");
  Serial.print(FSM.currentState);
  Serial.print("\t");
  Serial.print(FSM.nextState);
  Serial.print("\t");
  Serial.print(FSM.stateTransition);
  Serial.print("\t");
  Serial.print(FSM.gameMode);
  Serial.print("\t");
  Serial.print("\r\n");*/
  
}


void encoderPressed(){
  encoderInterrupt = true;
}

void resetPressed(){
  resetInterrupt = true;
}