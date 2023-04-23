#ifndef BASIC_FUNCTIONS
#define BASIC_FUNCTIONS

#include <stdint.h>


static volatile bool encoderInterrupt = false;
static volatile bool resetInterrupt = false;


uint8_t initArduino();

uint8_t setRGB(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);

char getChar();

uint8_t clearInput();

void encoderPressed();

void resetPressed();

#endif