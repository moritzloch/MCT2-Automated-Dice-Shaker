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

#include "config.h"
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
            playerLieDetectionStateFunction(FSM, menus[MENU_LIEDETECTION]);
            break;

        case ST_BEFORELIEDETECTION:
            beforeLieDetectionStateFunction(FSM);
            break;

        case ST_CHECKFORLIE1:
            checkForLieStateFunction1(FSM);
            break;

        case ST_CHECKFORLIE2:
            checkForLieStateFunction2(FSM);
            break;
        
        case ST_ENDOFTURN:
            endOfTurnStateFunction(FSM);
            break;

        case ST_ENDOFGAME:
            endOfGameStateFunction(FSM);
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
            getRandomDiceRoll(&FSM->diceRoll);
            if(checkIfGreater(FSM->diceRoll, FSM->prevDiceRoll)){
                FSM->trueNumberAnnounced = true;
            }
            else{
                while(!checkIfGreater(FSM->diceRoll, FSM->prevDiceRoll)){
                    getRandomDiceRoll(&FSM->diceRoll);
                }
                FSM->trueNumberAnnounced = false;
            }
            lcdPrintDiceNumber(FSM->diceRoll, &FSM->firstFrame);
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


uint8_t playerLieDetectionStateFunction(FsmProperties* FSM, MenuProperties* lieDetectionMenu){

    if(FSM->stateTransition){
        if(lieDetectionMenu->selectedIndex == 0){    //trust previous player
            FSM->nextState = ST_ENDOFTURN;
        }
        else FSM->nextState = ST_CHECKFORLIE1;
        resetMenuProperties(menus[MENU_LIEDETECTION], 2);
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(lieDetectionMenu, lieDetectionMenuItemNames, &FSM->firstFrame);

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


uint8_t checkForLieStateFunction1(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_CHECKFORLIE2;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->trueNumberAnnounced) lcdPrint("Falsch!", "", &FSM->firstFrame);
        else lcdPrint("Richtig!", "", &FSM->firstFrame);
    }

    return 0;
}


uint8_t checkForLieStateFunction2(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->diceRoll = 0;
        FSM->nextState = ST_ENDOFTURN;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->trueNumberAnnounced){
            FSM->lifeCount[FSM->nextPlayer]--;
            if(FSM->nextPlayer == 0) lcdPrint("Die CPU", "verliert Leben", &FSM->firstFrame);
            else lcdPrintPlayerNumber("", "verliert Leben", FSM->nextPlayer, &FSM->firstFrame);
        } 
        else{
            FSM->lifeCount[FSM->currentPlayer]--;
            if(FSM->currentPlayer == 0) lcdPrint("Die CPU", "verliert Leben", &FSM->firstFrame);
            else lcdPrintPlayerNumber("", "verliert Leben", FSM->nextPlayer, &FSM->firstFrame);
        }
    }

    return 0;
}


uint8_t endOfTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        checkForWinner(FSM);
        if(FSM->winningPlayer == -1){
            resetTurn(FSM);
            if(FSM->currentPlayer == 0) FSM->nextState = ST_CPUTURN;
            else FSM->nextState = ST_PLAYERTURN;
        }
        else FSM->nextPlayer = ST_ENDOFGAME;
        FSM->stateTransition = false;
    }
    else lcdPrint("End of", "Turn", &FSM->firstFrame);

    return 0;
}


uint8_t endOfGameStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_MENU;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->currentPlayer == 0) lcdPrint("Die CPU", "hat gewonnen", &FSM->firstFrame);
        else lcdPrintPlayerNumber("", "hat gweonnen", FSM->winningPlayer, &FSM->firstFrame);
    }

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
    resetMenuProperties(menus[MENU_LIEDETECTION], 2);

    FSM->firstFrame = true;
    FSM->currentState = ST_MENU;
    FSM->nextState = ST_MENU;
    FSM->stateTransition = false;
    FSM->numberOfLives = STANDARDNUMBEROFLIVES;
    FSM->numberOfPlayers = STANDARDNUMBEROFPLAYERS;
    FSM->prevDiceRoll = 0;
    FSM->diceRoll = 0;
    FSM->winningPlayer = -1;

    for(uint8_t i = 0; i < FSM->numberOfPlayers + 1; i++){
        FSM->lifeCount[i] = FSM->numberOfLives;
        Serial.print(FSM->lifeCount[i]);
    }

    for(uint8_t i = 0; i < FSM->numberOfPlayers + 1; i++){
        Serial.print(FSM->lifeCount[i]);
    }

    return 0;
}


uint8_t resetTurn(FsmProperties* FSM){

    FSM->currentPlayer = FSM->nextPlayer;
    while(!(FSM->lifeCount[FSM->currentPlayer] > 0)){
        FSM->currentPlayer += 1;
        if(FSM->currentPlayer > FSM->numberOfPlayers) FSM->currentPlayer = 0;
        //Serial.print(FSM->currentPlayer);Serial.print(FSM->nextPlayer);Serial.println(FSM->players[FSM->currentPlayer]->lifeCount);
    }

    FSM->nextPlayer = FSM->currentPlayer + 1;
    if(FSM->nextPlayer > FSM->numberOfPlayers) FSM->nextPlayer = 0;
    while(!(FSM->lifeCount[FSM->nextPlayer] > 0)){
        FSM->nextPlayer += 1;
        if(FSM->nextPlayer > FSM->numberOfPlayers) FSM->nextPlayer = 0;
    }
    

    FSM->prevDiceRoll = FSM->diceRoll;

    return 0;
}


uint8_t checkForWinner(FsmProperties* FSM){

    uint8_t playerWithRemainingLives = 0;
    uint8_t winningPlayer;

    for(uint8_t i = 0; i < FSM->numberOfPlayers; i++){
        if(FSM->lifeCount[i] > 0){
            playerWithRemainingLives++;
            winningPlayer = i;
        }
    }

    if(playerWithRemainingLives == 1) FSM->winningPlayer = winningPlayer;

    return 0;
}