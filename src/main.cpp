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


volatile bool encoderInterrupt = false;
volatile bool resetInterrupt = false;

void setup(){

  initArduino();
  resetMenuProperties(mainMenuProperties, 3);
  resetMenuProperties(gameSettingsMenuProperties, 4);

}

void loop(){

  if(encoderInterrupt){
    FSM.stateTransition = true;
    delay(200);
    encoderInterrupt = false;
  }
  if(resetInterrupt){
    resetMenuProperties(mainMenuProperties, 3);
    resetMenuProperties(gameSettingsMenuProperties, 4);
    FSM.currentState = ST_MENU;
    FSM.nextState = ST_MENU;
    FSM.stateTransition = false;
    delay(200);
    resetInterrupt = false;
  }

  switch(FSM.currentState){

        case ST_MENU:
            lcdScrollMenu(mainMenuProperties, mainMenuItemNames);
            if(FSM.stateTransition){
                FSM.gameMode = (gameMode_t) mainMenuProperties.selectedIndex;
                if(FSM.gameMode == GM_MAEXLE) FSM.nextState = ST_GAMESETTINGS;
                else if(FSM.gameMode == GM_DICEROLL) FSM.nextState = ST_DICEROLL;
                else FSM.nextState = ST_MENU;
                FSM.stateTransition = false;
            }
            break;

        case ST_GAMESETTINGS:
            lcdScrollMenu(gameSettingsMenuProperties, gameSettingsMenuItemNames);
            if(FSM.stateTransition){
                if(gameSettingsMenuProperties.selectedIndex == 0){
                  randomSeed(analogRead(0));
                  FSM.nextState = (state_t) random(2, 4);
                }
                FSM.stateTransition = false;
            }
            break;

        case ST_CPUTURN:
            lcdPrint("CPU");
            if(FSM.stateTransition){
              FSM.nextState = ST_PLAYERTURN;
              FSM.stateTransition = false;
            }
            break;

        case ST_PLAYERTURN:
            lcdPrint("PLAYER");
            if(FSM.stateTransition){
              FSM.nextState = ST_CPUTURN;
              FSM.stateTransition = false;
            }
            break;

        case ST_DICEROLL:
            lcdPrint("Wuerfeln");
            break;
    }

    FSM.currentState = FSM.nextState;


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