#ifndef GAMESTATE_FUNCTIONS
#define GAMESTATE_FUNCTIONS

#include <stdint.h>
#include "menuFunctions.h"


/**
 * @brief Definition of all possible FSM states
 * 
 */
enum state_t{
    ST_MENU,
    ST_GAMESETTINGS,
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
    ST_DICEROLL
};


enum gameMode_t{
    GM_MAEXLE,
    GM_DICEROLL,
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
    uint8_t numberOfLives;
    uint8_t numberOfPlayers;
    uint8_t lifeCount[9];
    //uint8_t numberOfDice;
    uint8_t currentPlayer;
    uint8_t nextPlayer;
    int8_t prevDiceRollIndex;
    int8_t diceRollIndex;
    bool trueNumberAnnounced;
    int8_t winningPlayer = -1;
};
static FsmProperties fsm;



uint8_t gameStateFSM(FsmProperties* FSM, MenuProperties** menus);

uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu);

uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu);

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

uint8_t diceRollStateFunction(FsmProperties* FSM);

uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus);

uint8_t resetTurn(FsmProperties* FSM);

uint8_t checkForWinner(FsmProperties* FSM);

#endif