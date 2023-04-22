#ifndef GAMESTATE_FUNCTIONS
#define GAMESTATE_FUNCTIONS

#include <stdint.h>


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
    state_t currentState = ST_MENU;
    state_t nextState = ST_MENU;
    gameMode_t gameMode;
};


static FsmProperties FSM;


uint8_t gameStateFSM();

uint8_t mainMenu();

uint8_t resetFSM();

#endif