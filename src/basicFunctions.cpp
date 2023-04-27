/**
 * @file basicFunctions.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Basic Functions for initialising the program and handling pseudorandom numbers
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
 * @brief initialising Serial Monitor, pins & interrupts
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

    pinMode(RE_SW, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(RE_SW), encoderPressed, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RESET), resetPressed, CHANGE);

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


/**
 * @brief Get random "Mäxle" dice roll
 * 
 * @param diceRollIndex Index of current dice roll
 * @return uint8_t 0
 */
uint8_t getMaexleDiceRoll(int8_t* diceRollIndex){

  uint8_t diceRoll;

  randomSeed(analogRead(0));
  uint8_t digit1 = random(1, 7);
  delay(10);
  randomSeed(analogRead(0));
  uint8_t digit2 = random(1, 7);

  if(digit1 > digit2) diceRoll = (10 * digit1) + digit2;
  else diceRoll = (10 * digit2) + digit1;

  *diceRollIndex = valueToIndexLUT[diceRoll];

  return 0;
}


/**
 * @brief Get random dice roll with custom dice & eyes number
 * 
 * @param diceRoll Array of values of dice rolls 
 * @param nrDice Number of rolled dice
 * @param nrEyes Number of maximum dice eyes
 * @return uint8_t 0
 */
uint8_t getCustomDiceRoll(uint8_t* diceRoll, uint8_t nrDice, uint8_t nrEyes){

  for(uint8_t i = 0; i < nrDice; i++){
    randomSeed(analogRead(0));
    diceRoll[i] = random(1, nrEyes + 1);
    delay(10);
  }

  return 0;
}


/**
 * @brief Get random grade lower than maximum grade
 * 
 * @param grade Value of grde
 * @param maxGrade Maximum achieveable grade
 * @return uint8_t 0
 */
uint8_t getGrade(float* grade, float maxGrade){

  uint8_t maxGradeOne = floor(maxGrade);
  uint8_t maxGradeDec = round(10 * (maxGrade - maxGradeOne));

  Serial.print(maxGrade);
  Serial.print("\t");
  Serial.print(maxGradeOne);
  Serial.print("\t");
  Serial.print(maxGradeDec);
  Serial.print("\t");
  Serial.print(maxGrade - maxGradeOne);
  Serial.print("\t");
  Serial.print(10*(maxGrade - maxGradeOne));
  Serial.print("\t");
  Serial.print((uint8_t) (10*(maxGrade - maxGradeOne)));
  Serial.print("\t");

  randomSeed(analogRead(0));
  uint8_t digit1 = random(maxGradeOne, 6);
  delay(10);
  randomSeed(analogRead(0));
  uint8_t digit2;
  if(digit1 == 5) digit2 = 0;
  else if(digit1 == maxGradeOne) digit2 = random(maxGradeDec, 10);
  else digit2 = random(0, 10);

  Serial.print(digit1);
  Serial.print("\t");
  Serial.print(digit2);
  Serial.print("\t");

  *grade = digit1 + 0.1 * digit2;

  Serial.print(*grade);
  Serial.print("\t");
  Serial.print("\r\n");

  return 0;
}



/**
 * ! DEPRECATED ! "Mäxle" dice roll are stored as indeces
 * @brief Check for greater "Mäxle" value
 * 
 * @param value "Mäxle" value
 * @param compValue "Mäxle" comparison value 
 * @return true when value >= compValue
 * @return false when value < compValue
 */
bool checkIfGreater(uint8_t value, uint8_t compValue){

  bool retValue;

  if(value == 21) retValue = true;
  else if(((value % 11) == 0) && (!((compValue % 11) == 0))) retValue = true;
  else if((!(value % 11) == 0) && (((compValue % 11) == 0))) retValue = false;
  else retValue = (value > compValue);

  return retValue;
}