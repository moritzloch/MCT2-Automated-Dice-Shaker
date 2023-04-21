#ifndef CUSTOMLCDCHARACTERS
#define CUSTOMLCDCHARACTERS

#include "Arduino.h"

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