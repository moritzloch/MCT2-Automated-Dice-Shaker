/**
 * @file basicFunctions.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Basic Functions for initialising the program
 * @version 1.0
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdint.h>
#include <Arduino.h>

#include "config.h"
#include "basicFunctions.h"


/**
 * @brief initialising Serial Monitor and pins
 * 
 * @return uint8_t 
 */
uint8_t initArduino(){

    Serial.begin(9600);

    return 0;
}