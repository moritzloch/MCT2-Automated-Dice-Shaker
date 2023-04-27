#ifndef GAMESTATE_FUNCTIONS
#define GAMESTATE_FUNCTIONS
/**
 * @file gameStateFunctions.h
 * @author Marco Schweizer, Moritz Loch
 * @brief Defining FSM states, properties & functions
 * @version 1.0
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>
#include "menuFunctions.h"


/**
 * @brief Enum FSM states
 * 
 */
enum state_t{
    ST_MENU,
    ST_GAMESETTINGS,
    ST_GMSTNRPLAYERS,
    ST_GMSTNRLIVES,
    ST_CPUTURN,
    ST_CPUDICEROLL,
    ST_CPULIEDETECTION,
    ST_PLAYERTURN,
    ST_PLAYERDICEROLL,
    ST_PLAYERLIEDETECTION,
    ST_PLAYERLIECONFIRMATION1,
    ST_PLAYERLIECONFIRMATION2,
    ST_BEFORELIEDETECTION,
    ST_CHECKFORLIE1,
    ST_CHECKFORLIE2,
    ST_CHECKFORLIE3,
    ST_ENDOFTURN,
    ST_ENDOFGAME,
    ST_DICEROLLMENU,
    ST_DICEROLL,
    ST_DRMNRDICE,
    ST_DRMNREYES,
    ST_GRADEMENU,
    ST_GRMAXGRADE,
    ST_GRADE,
    ST_SETTINGSMENU,
    ST_STAUTODICEROLL
};


/**
 * @brief Enum game modes
 * 
 */
enum gameMode_t{
    GM_MAEXLE,
    GM_DICEROLL,
    GM_GRADES,
    GM_SETTINGS
};


/**
 * @brief Stores all current data of FSM
 * 
 */
struct FsmProperties{
    bool stateTransition = false;
    bool firstFrame = true;
    state_t currentState = ST_MENU;
    state_t nextState = ST_MENU;
    gameMode_t gameMode;

    int8_t numberOfLives;
    int8_t numberOfPlayers;
    uint8_t lifeCount[9];
    uint8_t currentPlayer;
    uint8_t nextPlayer;
    int8_t prevDiceRollIndex;
    int8_t diceRollIndex;
    bool trueNumberAnnounced;
    int8_t winningPlayer = -1;

    int8_t numberOfDice;
    int8_t numberOfEyes;

    float maxGrade = DEFAULTMAXGRADE;

    bool autoDiceRoll = DEFAULTAUTODICEROLL;
};
static FsmProperties fsm;



uint8_t gameStateFSM(FsmProperties* FSM, MenuProperties** menus);

uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu);

uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu);

uint8_t gmStNrPlayersStateFunction(FsmProperties* FSM);

uint8_t gmStNrLivesStateFunction(FsmProperties* FSM);

uint8_t cpuTurnStateFunction(FsmProperties* FSM);

uint8_t cpuDiceRollStateFunction(FsmProperties* FSM);

uint8_t cpuLieDetectionStateFunction(FsmProperties* FSM);

uint8_t playerTurnStateFunction(FsmProperties* FSM);

uint8_t playerDiceRollStateFunction(FsmProperties* FSM);

uint8_t beforeLieDetectionStateFunction(FsmProperties* FSM);

uint8_t playerLieDetectionStateFunction(FsmProperties* FSM, MenuProperties* lieDetectionMenu);

uint8_t playerLieConfirmationStateFunction1(FsmProperties* FSM);

uint8_t playerLieConfirmationStateFunction2(FsmProperties* FSM, MenuProperties* lieDetectionMenu);

uint8_t checkForLieStateFunction1(FsmProperties* FSM);

uint8_t checkForLieStateFunction2(FsmProperties* FSM);

uint8_t checkForLieStateFunction3(FsmProperties* FSM);

uint8_t endOfTurnStateFunction(FsmProperties* FSM);

uint8_t endOfGameStateFunction(FsmProperties* FSM);

uint8_t diceRollMenuStateFunction(FsmProperties* FSM, MenuProperties* diceRollMenu);

uint8_t drmNrDiceStateFunction(FsmProperties* FSM);

uint8_t drmNrEyesStateFunction(FsmProperties* FSM);

uint8_t diceRollStateFunction(FsmProperties* FSM);

uint8_t gradeMenuStateFunction(FsmProperties* FSM, MenuProperties* gradeMenu);

uint8_t grMaxGradeStateFunction(FsmProperties* FSM);

uint8_t gradeStateFunction(FsmProperties* FSM);

uint8_t settingsMenuStateFunction(FsmProperties* FSM, MenuProperties* settingsMenu);

uint8_t stAutoDiceRollStateFunction(FsmProperties* FSM, MenuProperties* autoDiceRollMenu);


uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus);

uint8_t resetTurn(FsmProperties* FSM);

uint8_t checkForWinner(FsmProperties* FSM);

#endif