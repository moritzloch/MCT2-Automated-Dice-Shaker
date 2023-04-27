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
 * @param FSM Properties of FSM
 * @param menus Properties of menus
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
        
        case ST_GMSTNRPLAYERS:
            gmStNrPlayersStateFunction(FSM);
            break;

        case ST_GMSTNRLIVES:
            gmStNrLivesStateFunction(FSM);
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

        case ST_BEFORELIEDETECTION:
            beforeLieDetectionStateFunction(FSM);
            break;

        case ST_PLAYERLIEDETECTION:
            playerLieDetectionStateFunction(FSM, menus[MENU_LIEDETECTION]);
            break;

        case ST_PLAYERLIECONFIRMATION1:
            playerLieConfirmationStateFunction1(FSM);
            break;
        
        case ST_PLAYERLIECONFIRMATION2:
            playerLieConfirmationStateFunction2(FSM, menus[MENU_LIEDETECTION]);
            break;

        case ST_CHECKFORLIE1:
            checkForLieStateFunction1(FSM);
            break;

        case ST_CHECKFORLIE2:
            checkForLieStateFunction2(FSM);
            break;

        case ST_CHECKFORLIE3:
            checkForLieStateFunction3(FSM);
            break;
        
        case ST_ENDOFTURN:
            endOfTurnStateFunction(FSM);
            break;

        case ST_ENDOFGAME:
            endOfGameStateFunction(FSM);
            break;

        case ST_DICEROLLMENU:
            diceRollMenuStateFunction(FSM, menus[MENU_DICEROLL]);
            break;

        case ST_DRMNRDICE:
            drmNrDiceStateFunction(FSM);
            break;

        case ST_DRMNREYES:
            drmNrEyesStateFunction(FSM);
            break;

        case ST_DICEROLL:
            diceRollStateFunction(FSM);
            break;

        case ST_GRADEMENU:
            gradeMenuStateFunction(FSM, menus[MENU_GRADE]);
            break;

        case ST_GRMAXGRADE:
            grMaxGradeStateFunction(FSM);
            break;

        case ST_GRADE:
            gradeStateFunction(FSM);
            break;

        case ST_SETTINGSMENU:
            settingsMenuStateFunction(FSM, menus[MENU_SETTINGS]);
            break;

        case ST_STAUTODICEROLL:
            stAutoDiceRollStateFunction(FSM, menus[MENU_AUTODICEROLL]);
            break;

        default:
            mainMenuStateFunction(FSM, menus[MENU_MAIN]);
            break;
    }

    FSM->currentState = FSM->nextState;

    return 0;
}


/**
 * @brief Function in main menu state: Selection of game mode
 * 
 * @param FSM Properties of FSM
 * @param mainMenu Properties of main menu
 * @return uint8_t 0
 */
uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu){

    if(FSM->stateTransition){
        FSM->gameMode = (gameMode_t) mainMenu->selectedIndex;
        switch(FSM->gameMode){
            case GM_MAEXLE: 
                FSM->nextState = ST_GAMESETTINGS;
                break;
            case GM_DICEROLL: 
                FSM->nextState = ST_DICEROLLMENU;
                break;
            case GM_GRADES:
                FSM->nextState = ST_GRADEMENU;
                break;
            case GM_SETTINGS: 
                FSM->nextState = ST_SETTINGSMENU;
                break;
            default: 
                FSM->nextState = FSM->currentState;
                break;
        }

        FSM->stateTransition = false;
    }
    else lcdScrollMenu(mainMenu, mainMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in game settings state: Start or configure "Mäxle"
 * 
 * @param FSM Properties of FSM
 * @param gameSettingsMenu Properties of game settings menu
 * @return uint8_t 0
 */
uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu){

    if(FSM->stateTransition){
        switch(gameSettingsMenu->selectedIndex){
            case 0:
                for(uint8_t i = 0; i < FSM->numberOfPlayers + 1; i++){
                    if(i < (FSM->numberOfPlayers + 1)) FSM->lifeCount[i] = FSM->numberOfLives;
                    else FSM->lifeCount[i] = 0;
                }
                randomSeed(analogRead(0));
                FSM->nextPlayer = (state_t) random(0, FSM->numberOfPlayers + 1);
                resetTurn(FSM);
                if(FSM->currentPlayer == 0) FSM->nextState = ST_CPUTURN;
                else FSM->nextState = ST_PLAYERTURN;

                break;

            case 1:
                FSM->nextState = ST_GMSTNRPLAYERS;
                break;

            case 2: 
                FSM->nextState = ST_GMSTNRLIVES;
                break;

            default:
                FSM->nextState = FSM->currentState;
                break;
        }
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(gameSettingsMenu, gameSettingsMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in game settings number of player state: Selection of number of players in "Mäxle"
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t gmStNrPlayersStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_GAMESETTINGS;
        FSM->stateTransition = false;
    }
    else lcdValueMenu("Anzahl Spieler:", &FSM->firstFrame, 0, 8, FSM->numberOfPlayers);

    return 0;
}


/**
 * @brief Function in game settings number of lives state: Selection of number of lives in "Mäxle"
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t gmStNrLivesStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_GAMESETTINGS;
        FSM->stateTransition = false;
    }
    else lcdValueMenu("Anzahl Leben:", &FSM->firstFrame, 0, 99, FSM->numberOfLives);

    return 0;
}


/**
 * @brief Function in cpu turn state: Notify of cpu turn
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t cpuTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_CPUDICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrint("Die CPU ist", "an der Reihe", &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in cpu dice roll state: Print random "Mäxle" dice roll
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t cpuDiceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_BEFORELIEDETECTION;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->firstFrame){
            getMaexleDiceRoll(&FSM->diceRollIndex);
            if(FSM->diceRollIndex > FSM->prevDiceRollIndex){
                FSM->trueNumberAnnounced = true;
            }
            else{
                while(!(FSM->diceRollIndex > FSM->prevDiceRollIndex)){
                    getMaexleDiceRoll(&FSM->diceRollIndex);
                }
                FSM->trueNumberAnnounced = false;
            }
            lcdPrintMaexleDiceNumber(indexToValueLUT[FSM->diceRollIndex], &FSM->firstFrame);
        }
    }

    return 0;
}


/**
 * @brief Function in cpu lie detection state: CPU randomly determines if previous dice roll is true/false
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t cpuLieDetectionStateFunction(FsmProperties* FSM){

    static bool guessTrueNumberAnnounced;

    if(FSM->stateTransition){
        if(guessTrueNumberAnnounced) FSM->nextState = ST_ENDOFTURN;
        else FSM->nextState = ST_PLAYERLIECONFIRMATION1;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->firstFrame){
            if(indexToValueLUT[FSM->diceRollIndex] == 21){
                guessTrueNumberAnnounced = false;
            }
            else{
                randomSeed(analogRead(0));
                guessTrueNumberAnnounced = random(0, 2);
            }

            if(guessTrueNumberAnnounced) lcdPrint("CPU sagt,", "das ist wahr", &FSM->firstFrame);
            else lcdPrint("CPU sagt", "das ist gelogen", &FSM->firstFrame);

            FSM->firstFrame = false;
        }
    }

    return 0;
}


/**
 * @brief Function in player turn state: Notify of player turn
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t playerTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_PLAYERDICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrintPlayerNumber("ist", "an der Reihe", FSM->currentPlayer, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in player dice roll state: Player selects announced number (Minimum: previous number)
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t playerDiceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_BEFORELIEDETECTION;
        FSM->stateTransition = false;
    }
    else{
        lcdDiceValueMenu("Meine Zahl:", &FSM->firstFrame, FSM->prevDiceRollIndex + 1, FSM->diceRollIndex);
    }

    return 0;
}


/**
 * @brief Function in before lie detection state: Determine and notify next player/cpu that has to decide
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
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


/**
 * @brief Function in player lie detection state: Player decides wheter previous player said the truth/lied
 * 
 * @param FSM Properties of FSM
 * @param lieDetectionMenu Properties of lie detection menu
 * @return uint8_t 0
 */
uint8_t playerLieDetectionStateFunction(FsmProperties* FSM, MenuProperties* lieDetectionMenu){

    if(FSM->stateTransition){
        if(lieDetectionMenu->cursorPos == 0) FSM->nextState = ST_ENDOFTURN;
        else{
            if(FSM->currentPlayer == 0) FSM->nextState = ST_CHECKFORLIE1;
            else FSM->nextState = ST_PLAYERLIECONFIRMATION1;
        }
        resetMenuProperties(lieDetectionMenu, 2);
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(lieDetectionMenu, lieDetectionMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in player lie confirmation state 1: Ask player for decision
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t playerLieConfirmationStateFunction1(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_PLAYERLIECONFIRMATION2;
        FSM->stateTransition = false;
    }
    else lcdPrintPlayerNumber(", was", "hast du gesagt?", FSM->currentPlayer, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in player lie confirmation state 2: Player confirms wheter they said the truth/lied
 * 
 * @param FSM Properties of FSM
 * @param lieDetectionMenu Properties of lie detection menu
 * @return uint8_t 0
 */
uint8_t playerLieConfirmationStateFunction2(FsmProperties* FSM, MenuProperties* lieDetectionMenu){

    if(FSM->stateTransition){
        FSM->trueNumberAnnounced = !lieDetectionMenu->selectedIndex;
        FSM->nextState = ST_CHECKFORLIE1; 
        resetMenuProperties(lieDetectionMenu, 2);
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(lieDetectionMenu, lieDetectionMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in check for lie state 1: Print player/cpu that was wrong
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t checkForLieStateFunction1(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_CHECKFORLIE2;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->trueNumberAnnounced){
            if(FSM->nextPlayer == 0) lcdPrint("Die CPU", "lag falsch", &FSM->firstFrame);
            else lcdPrintPlayerNumber("", "lag falsch", FSM->nextPlayer, &FSM->firstFrame);
        }
        else{
            if(FSM->nextPlayer == 0) lcdPrint("Die CPU", "lag richtig", &FSM->firstFrame);
            else lcdPrintPlayerNumber("", "lag richtig", FSM->nextPlayer, &FSM->firstFrame);
        }
    }

    return 0;
}


/**
 * @brief Function in check for lie state 2: Reduce life count of wrong player/cpu and print player/cpu that loses lives
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t checkForLieStateFunction2(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->diceRollIndex = -1;
        FSM->nextState = ST_CHECKFORLIE3;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->firstFrame){
            if(FSM->trueNumberAnnounced){
                FSM->lifeCount[FSM->nextPlayer]--;
                if(FSM->nextPlayer == 0) lcdPrint("Die CPU", "verliert Leben", &FSM->firstFrame);
                else lcdPrintPlayerNumber("", "verliert Leben", FSM->nextPlayer, &FSM->firstFrame);
            } 
            else{
                FSM->lifeCount[FSM->currentPlayer]--;
                if(FSM->currentPlayer == 0) lcdPrint("Die CPU", "verliert Leben", &FSM->firstFrame);
                else lcdPrintPlayerNumber("", "verliert Leben", FSM->currentPlayer, &FSM->firstFrame);
            }
            if(FSM->firstFrame) FSM->firstFrame = false;
        }
    }

    return 0;
}


/**
 * @brief Function in check for lie state 3: Print out new life count of player/cpu that lost life
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t checkForLieStateFunction3(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_ENDOFTURN;
        FSM->stateTransition = false;
    }
    else{
        if(FSM->firstFrame){
            if(FSM->trueNumberAnnounced){
                lcdPrintLives(FSM->nextPlayer, FSM->lifeCount[FSM->nextPlayer], &FSM->firstFrame);
            } 
            else{
                lcdPrintLives(FSM->currentPlayer, FSM->lifeCount[FSM->currentPlayer], &FSM->firstFrame);
            }
            if(FSM->firstFrame) FSM->firstFrame = false;
        }
    }

    return 0;
}


/**
 * @brief Function in end of turn state: Check if game is won and determine new current & next player based on life count
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t endOfTurnStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        checkForWinner(FSM);
        if(FSM->winningPlayer == -1){
            resetTurn(FSM);
            if(FSM->currentPlayer == 0) FSM->nextState = ST_CPUTURN;
            else FSM->nextState = ST_PLAYERTURN;
        }
        else FSM->nextState = ST_ENDOFGAME;
        FSM->stateTransition = false;
    }
    else FSM->stateTransition = true;

    return 0;
}


/**
 * @brief Function in end of game state: Print out winning player/cpu & reset FSM
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t endOfGameStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_MENU;
        resetFSM(FSM, menus);
        FSM->stateTransition = false;
    }
    else{
        if(FSM->winningPlayer == 0) lcdPrint("Die CPU", "hat gewonnen", &FSM->firstFrame);
        else lcdPrintPlayerNumber("", "hat gewonnen", FSM->winningPlayer, &FSM->firstFrame);
    }

    return 0;
}


/**
 * @brief Function in dice roll settings menu state: Start or configure "Würfeln"
 * 
 * @param FSM Properties of FSM
 * @param diceRollMenu Properties of dice roll menu
 * @return uint8_t 0
 */
uint8_t diceRollMenuStateFunction(FsmProperties* FSM, MenuProperties* diceRollMenu){

    if(FSM->stateTransition){
        switch(diceRollMenu->selectedIndex){
            case 0:
                FSM->nextState = ST_DICEROLL;
                break;
            case 1:
                FSM->nextState = ST_DRMNRDICE;
                break;
            case 2: 
                FSM->nextState = ST_DRMNREYES;
                break;
            default:
                FSM->nextState = FSM->currentState;
                break;
        }
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(diceRollMenu, diceRollMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in dice roll menu number of dice state: Selection of number of dice in "Würfeln"
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t drmNrDiceStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_DICEROLLMENU;
        FSM->stateTransition = false;
    }
    else lcdValueMenu("Anzahl Wuerfel:", &FSM->firstFrame, 1, 5, FSM->numberOfDice);

    return 0;
}


/**
 * @brief Function in dice roll menu number of eyes state: Selection of number of eyes (max. dice value) in "Würfeln"
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t drmNrEyesStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_DICEROLLMENU;
        FSM->stateTransition = false;
    }
    else lcdValueMenu("Max. Augenzahl:", &FSM->firstFrame, 1, 20, FSM->numberOfEyes);

    return 0;
}


/**
 * @brief Function in dice roll state: Print out random & custom dice roll
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t diceRollStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_DICEROLL;
        FSM->stateTransition = false;
    }
    else lcdPrintCustomDiceNumber(FSM->numberOfDice, FSM->numberOfEyes, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in grade menu state: Start or configure "Noten würfeln"
 * 
 * @param FSM Properties of FSM
 * @param gradeMenu Properties of grade menu
 * @return uint8_t 0
 */
uint8_t gradeMenuStateFunction(FsmProperties* FSM, MenuProperties* gradeMenu){

    if(FSM->stateTransition){
        switch(gradeMenu->selectedIndex){
            case 0:
                FSM->nextState = ST_GRADE;
                break;
            case 1:
                FSM->nextState = ST_GRMAXGRADE;
                break;
            default:
                FSM->nextState = FSM->currentState;
                break;
        }
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(gradeMenu, gradeMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in grade menu max. grade state: Selection of max. achievable grade in "Noten würfeln"
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */uint8_t grMaxGradeStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_GRADEMENU;
        FSM->stateTransition = false;
    }
    else lcdFloatValueMenu("Max. Bestnote:", &FSM->firstFrame, 1.0f, 5.0f, FSM->maxGrade);

    return 0;
}


/**
 * @brief Function in grade state: Print out random grade worse than max. achievable grade
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t gradeStateFunction(FsmProperties* FSM){

    if(FSM->stateTransition){
        FSM->nextState = ST_GRADE;
        FSM->stateTransition = false;
    }
    else lcdPrintGrade(FSM->maxGrade, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in settings menu state: Configure general settings
 * 
 * @param FSM Properties of FSM
 * @param settingsMenu Properties of settings menu
 * @return uint8_t 0
 */
uint8_t settingsMenuStateFunction(FsmProperties* FSM, MenuProperties* settingsMenu){

    if(FSM->stateTransition){
        switch(settingsMenu->selectedIndex){
            case 0:
                FSM->nextState = ST_MENU;
                break;
            case 1:
                FSM->nextState = ST_STAUTODICEROLL;
                break;
            default:
                FSM->nextState = FSM->currentState;
                break;
        }
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(settingsMenu, settingsMenuItemNames, &FSM->firstFrame);

    return 0;
}


/**
 * @brief Function in settings auto dice roll state: Set automatic (hardware) dice roll on/off
 * 
 * @param FSM Properties of FSM
 * @param autoDiceRoLlMenu Properties of auto dice roll menu
 * @return uint8_t 0
 */
uint8_t stAutoDiceRollStateFunction(FsmProperties* FSM, MenuProperties* autoDiceRollMenu){

    if(FSM->stateTransition){
        autoDiceRollMenu->selectedIndex = FSM->autoDiceRoll;
        FSM->nextState = ST_SETTINGSMENU;
        FSM->stateTransition = false;
    }
    else lcdScrollMenu(autoDiceRollMenu, autoDiceRollMenuItemNames, &FSM->firstFrame);

    return 0;
}



/**
 * @brief Resets FSM to default values
 * 
 * @param FSM Properties of FSM
 * @param menus Array of menu properties of all menus
 * @return uint8_t 0
 */
uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus){

    resetMenuProperties(menus[MENU_MAIN], 4);
    resetMenuProperties(menus[MENU_GAMESETTINGS], 3);
    resetMenuProperties(menus[MENU_LIEDETECTION], 2);
    resetMenuProperties(menus[MENU_DICEROLL], 3);
    resetMenuProperties(menus[MENU_GRADE], 2);
    resetMenuProperties(menus[MENU_SETTINGS], 2);
    resetMenuProperties(menus[MENU_AUTODICEROLL], 2);

    FSM->firstFrame = true;
    FSM->currentState = ST_MENU;
    FSM->nextState = ST_MENU;
    FSM->stateTransition = false;

    FSM->numberOfLives = DEFAULTNUMBEROFLIVES;
    FSM->numberOfPlayers = DEFAULTNUMBEROFPLAYERS;
    FSM->prevDiceRollIndex = -1;
    FSM->diceRollIndex = -1;
    FSM->winningPlayer = -1;

    FSM->numberOfDice = DEFAULTNUMBEROFDICE;
    FSM->numberOfEyes = DEFAULTNUMBEROFEYES;

    FSM->maxGrade = DEFAULTMAXGRADE;

    return 0;
}


/**
 * @brief Resets "Mäxle" turn: Determining new current & next player based on life count and setting new minimu dice value
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t resetTurn(FsmProperties* FSM){

    FSM->currentPlayer = FSM->nextPlayer;
    while(!(FSM->lifeCount[FSM->currentPlayer] > 0)){
        FSM->currentPlayer += 1;
        if(FSM->currentPlayer > FSM->numberOfPlayers) FSM->currentPlayer = 0;
    }

    FSM->nextPlayer = FSM->currentPlayer + 1;
    if(FSM->nextPlayer > FSM->numberOfPlayers) FSM->nextPlayer = 0;
    while(!(FSM->lifeCount[FSM->nextPlayer] > 0)){
        FSM->nextPlayer += 1;
        if(FSM->nextPlayer > FSM->numberOfPlayers) FSM->nextPlayer = 0;
    }
    
    FSM->prevDiceRollIndex = FSM->diceRollIndex;

    return 0;
}


/**
 * @brief Determines wheter all but one players/cpu have no remaining lives and game is won
 * 
 * @param FSM Properties of FSM
 * @return uint8_t 0
 */
uint8_t checkForWinner(FsmProperties* FSM){

    uint8_t playerWithRemainingLives = 0;
    uint8_t winningPlayer;

    for(uint8_t i = 0; i < FSM->numberOfPlayers + 1; i++){
        if(FSM->lifeCount[i] > 0){
            playerWithRemainingLives++;
            winningPlayer = i;
        }
    }

    if(playerWithRemainingLives == 1) FSM->winningPlayer = winningPlayer;

    return 0;
}