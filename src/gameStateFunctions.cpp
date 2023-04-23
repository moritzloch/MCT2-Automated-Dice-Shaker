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
uint8_t gameStateFSM(FsmProperties* FSM, MenuProperties** menus){

    switch(FSM->currentState){

        case ST_MENU:
            mainMenuStateFunction(FSM, menus[MENU_MAIN]);
            break;

        case ST_GAMESETTINGS:
            gameSettingsStateFunction(FSM, menus[MENU_GAMESETTINGS]);
            break;

        case ST_CPUTURN:
            cpuTurnStateFunction(FSM);
            break;

        case ST_PLAYERTURN:
            playerTurnStateFunction(FSM);
            break;

        case ST_DICEROLL:
            diceRollStateFunction(FSM);
            break;
    }

    FSM->currentState = FSM->nextState;

    return 0;
}


uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu){

    lcdScrollMenu(mainMenu, mainMenuItemNames);

    if(FSM->stateTransition){
        FSM->gameMode = (gameMode_t) mainMenu->selectedIndex;
        if(FSM->gameMode == GM_MAEXLE) FSM->nextState = ST_GAMESETTINGS;
        else if(FSM->gameMode == GM_DICEROLL) FSM->nextState = ST_DICEROLL;
        else FSM->nextState = ST_MENU;
        FSM->stateTransition = false;
    }

    return 0;
}


uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu){

    lcdScrollMenu(gameSettingsMenu, gameSettingsMenuItemNames);

    if(FSM->stateTransition){
        if(gameSettingsMenu->selectedIndex == 0){
            randomSeed(analogRead(0));
            FSM->nextState = (state_t) random(2, 4);
        }
        FSM->stateTransition = false;
    }

    return 0;
}


uint8_t cpuTurnStateFunction(FsmProperties* FSM){

    lcdPrint("CPU");

    if(FSM->stateTransition){
        FSM->nextState = ST_PLAYERTURN;
        FSM->stateTransition = false;
    }

    return 0;
}


uint8_t playerTurnStateFunction(FsmProperties* FSM){

    lcdPrint("PLAYER");

    if(FSM->stateTransition){
        FSM->nextState = ST_CPUTURN;
        FSM->stateTransition = false;
    }

    return 0;
}


uint8_t diceRollStateFunction(FsmProperties* FSM){

    lcdPrint("Wuerfeln");

    if(FSM->stateTransition){
        FSM->nextState = ST_DICEROLL;
        FSM->stateTransition = false;
    }

    return 0;
}


uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus){
    resetMenuProperties(menus[MENU_MAIN], 3);
    resetMenuProperties(menus[MENU_GAMESETTINGS], 4);
    FSM->currentState = ST_MENU;
    FSM->nextState = ST_MENU;
    FSM->stateTransition = false;

    return 0;
}