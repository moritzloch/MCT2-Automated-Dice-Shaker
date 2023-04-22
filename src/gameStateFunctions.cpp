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

#include "gameStateFunctions.h"


/**
 * @brief General Control FSM
 * 
 * @param currentState Current State of FSM
 * @return uint8_t 0
 */
uint8_t gameStateFSM(fsmData_t FSM){

    switch(FSM.currentState){

        case ST_MENU:
            //selectGameMode();
            break;
        case ST_GAMESETTINGS:

            break;
        case ST_CPUTURN:

            break;
        case ST_PLAYERTURN:

            break;
        case ST_DICEROLL:

            break;
    }

}


uint8_t selectGameMode(){

}
