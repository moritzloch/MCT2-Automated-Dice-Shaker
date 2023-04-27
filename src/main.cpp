/**
 * @file main.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Automated Dice Shaker
 * @version 1.0
 * @date 2023-04-27
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

  /**
   * @brief Encoder button press
   * 
   */
  if(encoderInterrupt){
    (&fsm)->stateTransition = true;
    (&fsm)->firstFrame = true;
    resetEncoder();
    delay(250);
    encoderInterrupt = false;
  }

  /**
   * @brief Reset button press
   * 
   */
  if(resetInterrupt){
    resetFSM(&fsm, menus);
    resetEncoder();
    delay(250);
    resetInterrupt = false;
  }

  /**
   * @brief Finite State Machine
   * 
   */
  gameStateFSM(&fsm, menus);

}


void encoderPressed(){
  encoderInterrupt = true;
}

void resetPressed(){
  resetInterrupt = true;
}