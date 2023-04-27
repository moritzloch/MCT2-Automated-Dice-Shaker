#ifndef CUSTOMLCDCHARACTERS
#define CUSTOMLCDCHARACTERS
/**
 * @file customLCDCharacters.h
 * @author Marco Schweizer, Moritz Loch
 * @brief Custom Characters for LCD-Display
 * @version 1.0
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Arduino.h"


/**
 * @brief Left arrow character
 * 
 */
enum customChar_t{
    arrowLeft
};
byte arrowLeftChar[8] = {
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b00111,
    0b00011,
    0b00001,
    0b00000,
};

#endif