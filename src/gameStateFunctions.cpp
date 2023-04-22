/**
 * @file gameStateFunctions.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief FSM Functions for general control
 * @version 1.0
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>
#include <Arduino.h>

#include "basicFunctions.h"
#include"menuFunctions.h"

#include "gameStateFunctions.h"


/**
 * @brief General Control FSM
 * 
 * @param currentState Current State of FSM
 * @return uint8_t 0
 */
uint8_t gameStateFSM(){

    switch(FSM.currentState){

        case ST_MENU:
            mainMenu();
            break;
        case ST_GAMESETTINGS:
            Serial.println("Mäxle");
            break;
        case ST_CPUTURN:

            break;
        case ST_PLAYERTURN:

            break;
        case ST_DICEROLL:
            Serial.println("Würfeln");
            break;
    }

    FSM.currentState = FSM.nextState;

    return 0;
}


uint8_t mainMenu(){

    lcdScrollMenu(mainMenuProperties, mainMenuItemNames);
    
    if(FSM.stateTransition){
        FSM.gameMode = (gameMode_t) mainMenuProperties.selectedIndex;
        if(FSM.gameMode == GM_MAEXLE) FSM.nextState = ST_GAMESETTINGS;
        else if(FSM.gameMode == GM_DICEROLL) FSM.nextState = ST_DICEROLL;
        else FSM.nextState = ST_MENU;
    }

    return 0;
}


uint8_t resetFSM(){
    resetMenuProperties(mainMenuProperties, 3);
    resetMenuProperties(gameSettingsMenuProperties, 4);
    FSM.currentState = ST_MENU;
    FSM.nextState = ST_MENU;
    FSM.stateTransition = false;
}