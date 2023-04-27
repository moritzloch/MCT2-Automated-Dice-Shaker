#ifndef BASIC_FUNCTIONS
#define BASIC_FUNCTIONS
/**
 * @file basicFunctions.h
 * @author Marco Schweizer, Moritz Loch
 * @brief Basic functions for I/O and pseudorandom numbers
 * @version 1.0
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>


/**
 * @brief LUT to convert "Mäxle" index to "Mäxle" dice value
 * 
 */
static const int8_t indexToValueLUT[] = {
    31, 32,
    41, 42, 43, 
    51, 52, 53, 54,
    61, 62, 63, 64, 65,
    11, 22, 33, 44, 55, 66, 
    21
};

/**
 * @brief LUT to convert "Mäxle" dice value to "Mäxle" index
 * 
 */
static const int8_t valueToIndexLUT[] = {
                                        -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    14, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    20, 15, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1, 16, -1, -1, -1, -1, -1, -1, -1,
     2,  3,  4, 17, -1, -1, -1, -1, -1, -1,
     5,  6,  7,  8, 18, -1, -1, -1, -1, -1,
     9, 10, 11, 12, 13, 19
};


static volatile bool encoderInterrupt = false;
static volatile bool resetInterrupt = false;


uint8_t initArduino();

uint8_t setRGB(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);

char getChar();

uint8_t clearInput();

void encoderPressed();

void resetPressed();

uint8_t getMaexleDiceRoll(int8_t* diceRoll);

uint8_t getCustomDiceRoll(uint8_t* diceRoll, uint8_t nrDice, uint8_t nrEyes);

uint8_t getGrade(float* grade, float maxGrade);

bool checkIfGreater(uint8_t value, uint8_t compValue);

#endif