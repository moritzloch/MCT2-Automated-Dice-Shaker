#ifndef TCS_FUNCTIONS
#define TCS_FUNCTIONS

#include <stdint.h>
#include <MD_TCS230.h>

#include "config.h"


//instance of TCS3200 color sensor
static MD_TCS230	TCS(TCS_S2, TCS_S3, TCS_0E);

//define readTypes for Read-FSM
#define READ_BLACK_CALIB    0
#define READ_WHITE_CALIB    1
#define READ_RGB            2


char getChar();

uint8_t clearInput();

uint8_t fsmReadValue(uint8_t state, uint8_t readType, colorData rgb);

#endif