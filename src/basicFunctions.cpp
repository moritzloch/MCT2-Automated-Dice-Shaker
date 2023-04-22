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
#include <Wire.h>

#include "config.h"
#include "tcsFunctions.h"
#include "menuFunctions.h"

#include "basicFunctions.h"


/**
 * @brief initialising Serial Monitor and pins
 * 
 * @return uint8_t 0
 */
uint8_t initArduino(){

    Serial.begin(9600);             //begin Serial monitor

    //set RGB pins as output
    /*pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    pinMode(RGB_RED, OUTPUT);*/

    //TCS.begin();                      //initiate TCS3200 color sensor

    initMenu();

    return 0;
}

/**
 * @brief Set color of RGB LED
 * 
 * @param redVal Value of red component in range 0 - 255
 * @param greenVal Value of green component in range 0 - 255
 * @param blueVal Value of blue component in range 0 - 255
 * @return uint8_t 0
 */
uint8_t setRGB(uint8_t redVal, uint8_t greenVal, uint8_t blueVal){

  analogWrite(RGB_RED, redVal);
  analogWrite(RGB_GREEN, greenVal);
  analogWrite(RGB_BLUE, blueVal);

  return 0;
}

/**
 * @brief Wait for Serial input while Serial buffer is empty 
 * 
 * @return Input char 
 */
char getChar(){

	while(Serial.available() == 0);

	return(toupper(Serial.read()));
}

/**
 * @brief Wait until all characters from serial input are cleared
 * 
 * @return uint8_t 0
 */
uint8_t clearInput(){

	while(Serial.read() != -1);

  return 0;
}