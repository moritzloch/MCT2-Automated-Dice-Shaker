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
#include "menuFunctions.h"

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
        
        case ST_CPUDICEROLL:
            cpuDiceRollStateFunction(FSM);
            break;

        case ST_CPULIEDETECTION:
            cpuLieDetectionStateFunction(FSM);
            break;

        case ST_PLAYERTURN:
            playerTurnStateFunction(FSM);
            break;

        case ST_PLAYERDICEROLL:
            playerDiceRollStateFunction(FSM);
            break;

        case ST_PLAYERLIEDETECTION:
            playerLieDetectionStateFunction(FSM);
            break;

        case ST_BEFORELIEDETECTION:
            beforeLieDetectionStateFunction(FSM);
            break;
        
        case ST_ENDOFTURN:
            endOfTurnStateFunction(FSM);
            break;

        case ST_DICEROLL:
            diceRollStateFunction(FSM);
            break;
    }

    FSM->currentState = FSM->nextState;

    return 0;
}


uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu){

    if(FSM->stateTransition){
        FSM->gameMode = (gameMode_t) mainMenu->selectedIndex;
        if(FSM->gameMode == GM_MAEXLE) FSM->nextState = ST_GAMESETTINGS;
        else if(FSM->gameMode == GM_DICEROLL) FSM->nextState = ST_DICEROLL;
        else FSM->nextState = ST_MENU;
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(mainMenu, mainMenuItemNames, &FSM->firstFrame);

    return 0;
}


uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu){

    if(FSM->stateTransition){
        if(gameSettingsMenu->selectedIndex == 0){
            randomSeed(analogRead(0));
            FSM->nextPlayer = (state_t) random(0, FSM->numberOfPlayers + 1);
            resetTurn(FSM);
            if(FSM->currentPlayer == 0) FSM->nextState = ST_CPUTURN;
            else FSM->nextState = ST_PLAYERTURN;
        }
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(gameSettingsMenu, gameSettingsMenuItemNames, &FSM->firstFrame);

    return 0;
}


uint8_t cpuTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_CPUDICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrint("Die CPU ist", "an der Reihe", &FSM->firstFrame);

    return 0;
}


uint8_t cpuDiceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_BEFORELIEDETECTION;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->firstFrame){
            getRandomDiceRoll(&FSM->actualDiceRoll);
            if(checkIfGreater(FSM->actualDiceRoll, FSM->prevDiceRoll)){
                lcdPrintDiceNumber(FSM->actualDiceRoll, &FSM->firstFrame);
            }
            else{
                while(!checkIfGreater(FSM->announcedDiceRoll, FSM->prevDiceRoll)){
                    getRandomDiceRoll(&FSM->announcedDiceRoll);
                }
                lcdPrintDiceNumber(FSM->announcedDiceRoll, &FSM->firstFrame);
                //FSM->announcedDiceRoll = 0;
            }
        }
    }

    return 0;
}


uint8_t cpuLieDetectionStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_ENDOFTURN;
        FSM->stateTransition = false;
    }
    else lcdPrint("CPU Lie", "Detection", &FSM->firstFrame);

    return 0;
}


uint8_t playerTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_PLAYERDICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrintPlayerNumber("ist", "an der Reihe", FSM->currentPlayer, &FSM->firstFrame);

    return 0;
}


uint8_t playerDiceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_BEFORELIEDETECTION;
        FSM->stateTransition = false;
    }
    else{
        lcdPrint("Player Dice", "Roll", &FSM->firstFrame);
    }

    return 0;
}


uint8_t playerLieDetectionStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_ENDOFTURN;
        FSM->stateTransition = false;
    }
    else lcdPrint("Player Lie", "Detection", &FSM->firstFrame);

    return 0;
}


uint8_t beforeLieDetectionStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        if(FSM->nextPlayer == 0) FSM->nextState = ST_CPULIEDETECTION;
        else FSM->nextState = ST_PLAYERLIEDETECTION;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->nextPlayer == 0) lcdPrint("Die CPU", "entscheidet", &FSM->firstFrame);
        else lcdPrintPlayerNumber("", "entscheidet", FSM->nextPlayer, &FSM->firstFrame);
    }

    return 0;
}


uint8_t endOfTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        resetTurn(FSM);
        if(FSM->currentPlayer == 0) FSM->nextState = ST_CPUTURN;
        else FSM->nextState = ST_PLAYERTURN;
        FSM->stateTransition = false;
    }
    else lcdPrint("End of", "Turn", &FSM->firstFrame);

    return 0;
}


uint8_t diceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_DICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrint("Wuerfeln", "", &FSM->firstFrame);

    return 0;
}


uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus){
    resetMenuProperties(menus[MENU_MAIN], 3);
    resetMenuProperties(menus[MENU_GAMESETTINGS], 4);
    FSM->firstFrame = true;
    FSM->currentState = ST_MENU;
    FSM->nextState = ST_MENU;
    FSM->stateTransition = false;
    FSM->numberOfLives = 3;
    FSM->numberOfPlayers = 2;
    FSM->prevDiceRoll = 22;
    FSM->actualDiceRoll = 0;
    FSM->announcedDiceRoll = 0;

    return 0;
}


uint8_t resetTurn(FsmProperties* FSM){

    FSM->currentPlayer = FSM->nextPlayer;
    FSM->nextPlayer = FSM->currentPlayer + 1;
    if(FSM->nextPlayer > FSM->numberOfPlayers) FSM->nextPlayer = 0;

    return 0;
}