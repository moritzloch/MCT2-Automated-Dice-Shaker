#ifndef GAMESTATE_FUNCTIONS
#define GAMESTATE_FUNCTIONS

#include <stdint.h>


/**
 * @brief Definition of all possible FSM states
 * 
 */
typedef enum state_t{
    ST_MENU,
    ST_GAMESETTINGS,
    ST_CPUTURN,
    ST_PLAYERTURN,
    ST_DICEROLL
};


typedef enum gameMode_t{
    GM_MAXLE,
    GM_DICEROLL,
    GM_SETTINGS
};


/**
 * @brief Stores all current data of FSM
 * 
 */
typedef struct fsmData_t{
    state_t currentState;
    state_t nextState;
};


#endif