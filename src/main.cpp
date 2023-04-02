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

#include "config.h"
#include "tcsFunctions.h"
#include "basicFunctions.h"


void setup(){

  initArduino();

}

void loop(){

  static uint8_t	runState = 0;		//state of run cycle, one time black/white calibration
  static uint8_t	readState = 0;  //state of read cycle, gets reset
  colorData rgb;

  switch(runState){
    case 0:	//calibrate black
      readState = fsmReadValue(readState, READ_BLACK_CALIB, rgb);
      if(readState == 0) runState++;
      break;

    case 1:	//calibrate white
      readState = fsmReadValue(readState, READ_WHITE_CALIB, rgb);
      if(readState == 0) runState++;
      break;

    case 2:	//read color
      readState = fsmReadValue(readState, READ_RGB, rgb);
      break;

    default:
      runState = 0;	//start again if we get here as something is wrong
  }

}