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
    ST_PLAYERTURN,
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
    //uint8_t numberOfDice;
};
static FsmProperties fsm;


struct PlayerProperties{
    uint8_t lifeCount;  
};
static PlayerProperties* players[9];


uint8_t gameStateFSM(FsmProperties* FSM, MenuProperties** menus);

uint8_t mainMenuStateFunction(FsmProperties* FSM, MenuProperties* mainMenu);

uint8_t gameSettingsStateFunction(FsmProperties* FSM, MenuProperties* gameSettingsMenu);

uint8_t cpuTurnStateFunction(FsmProperties* FSM);

uint8_t playerTurnStateFunction(FsmProperties* FSM);

uint8_t diceRollStateFunction(FsmProperties* FSM);

uint8_t resetFSM(FsmProperties* FSM, MenuProperties** menus);

#endif