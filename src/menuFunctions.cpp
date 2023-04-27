/**
 * @file menuFunctions.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Functions for LCD-Scroll Menus
 * @version 1.0
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Encoder.h>

#include "config.h"
#include "basicFunctions.h"
#include "customLCDCharacters.h"
#include "gameStateFunctions.h"

#include "menuFunctions.h"


/**
 * @brief LCD-Display Entity
 * 
 * @return LiquidCrystal 
 */
static LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/**
 * @brief Rotary Encoder Entity
 * 
 * @return Encoder 
 */
static Encoder encoder(RE_CLK, RE_DT);


/**
 * @brief Initiate LCD-Display & create custom characters
 * 
 * @return uint8_t 0
 */
uint8_t initMenu(){

  lcd.begin(LCD_COLS, LCD_ROWS);
  createCustomLCDChars();

  return 0;
}


/**
 * @brief Print custom text on lcd display
 * 
 * @param firstLineText Text in first line
 * @param secondLineText Text in second line
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrint(const char* firstLineText, const char* secondLineText, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(firstLineText);
    lcd.setCursor(0, 1);
    lcd.print(secondLineText);
  }

  return 0;
}


/**
 * @brief Print player number & custom text on lcd display
 * 
 * @param firstLineText Text in remaining first line
 * @param secondLineText Text in second line
 * @param playerNumber Number of player that will be printed
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrintPlayerNumber(const char* firstLineText, const char* secondLineText, uint8_t playerNumber, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Spieler ");
    lcd.setCursor(8, 0);
    lcd.print(playerNumber);
    lcd.setCursor(9, 0);
    lcd.print(" ");
    lcd.setCursor(10, 0);
    lcd.print(firstLineText);
    lcd.setCursor(0, 1);
    lcd.print(secondLineText);
  }

  return 0;
}


/**
 * @brief Print "Mäxle" dice roll number
 * 
 * @param diceNumber "Mäxle" dice roll that will be printed
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrintMaexleDiceNumber(uint8_t diceNumber, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CPU Zahl:");
    lcd.setCursor(0, 1);
    lcd.print(diceNumber);
  }

  return 0;
}


/**
 * @brief Get & print custom random dice number(s)
 * 
 * @param nrDice Number of rolled dice
 * @param nrEyes Number of eyes (max. dice value)
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrintCustomDiceNumber(uint8_t nrDice, uint8_t nrEyes, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;

    uint8_t diceRoll[nrDice];
    getCustomDiceRoll(diceRoll, nrDice, nrEyes);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wuerfel:");
    uint8_t cursorPos = 0;
    for(uint8_t i = 0; i < nrDice; i++){
      if(diceRoll[i] < 10) cursorPos++;

      lcd.setCursor(cursorPos, 1);
      lcd.print(diceRoll[i]);

      if(diceRoll[i] < 10) cursorPos += 2;
      else cursorPos += 3;
    }
  }

  return 0;
}


/**
 * @brief Print number of remaining lives in "Mäxle"
 * 
 * @param playerNumber Player number who's life count wil be printed
 * @param lives Array with life counts of all players/cpu
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrintLives(uint8_t playerNumber, uint8_t lives, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;
    if(playerNumber == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Die CPU hat");
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Spieler ");
      lcd.setCursor(8, 0);
      lcd.print(playerNumber);
      lcd.setCursor(9, 0);
      lcd.print(" hat");
    }

    if(lives > 0){
        lcd.setCursor(0, 1);
        lcd.print("noch ");
        lcd.setCursor(5, 1);
        lcd.print(lives);
      if(lives < 10){
        lcd.setCursor(6, 1);
        lcd.print(" Leben");
      }
      else{
        lcd.setCursor(7, 1);
        lcd.print(" Leben");
      }
    }
    else{
      lcd.setCursor(0, 1);
      lcd.print("keine Leben mehr");
    }

  }

  return 0;
}


/**
 * @brief Get & print random grade
 * 
 * @param maxGrade Max. achievable value of grade
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdPrintGrade(float maxGrade, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;

    float grade;
    getGrade(&grade, maxGrade);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Note:");
    lcd.setCursor(0,1);
    lcd.print(grade);
    lcd.setCursor(3, 1);
    lcd.print(" ");
  }

  return 0;
}


/**
 * @brief Create custom characters for lcd display
 * 
 * @return uint8_t 0
 */
uint8_t createCustomLCDChars(){

  lcd.createChar(arrowLeft, arrowLeftChar);

  return 0;
}


/**
 * @brief Reset menu properties to default value
 * 
 * @param menuProperties Properties of menu that will be reset
 * @param itemNumber Number items in menu
 * @return uint8_t 0
 */
uint8_t resetMenuProperties(MenuProperties* menuProperties, uint8_t itemNumber){
  menuProperties->cursorPos = 0;
  menuProperties->itemNumber = itemNumber;
  menuProperties->topIndex = 0;
  menuProperties->selectedIndex = 0;

  return 0;
}


/**
 * @brief Reset rotary encoder position to zero
 * 
 * @return uint8_t 0
 */
uint8_t resetEncoder(){
  encoder.write(0);
  return 0;
}


/**
 * @brief Custom scroll selection menu on lcd display
 * 
 * @param menuProperties Menu from which theitems will be selected
 * @param menuItemNames Names of the items in the menu
 * @param firstFrame First frame of FSM state flag
 * @return uint8_t 0
 */
uint8_t lcdScrollMenu(MenuProperties* menuProperties, const char** menuItemNames, bool* firstFrame){

  int8_t encoderDirection = encoder.read() / 4;

  if(((menuProperties->cursorPos == 0) && (encoderDirection > 0)) || ((menuProperties->cursorPos == 1) && (encoderDirection < 0))){
    menuProperties->cursorPos ^= 1;
  }
  else if((menuProperties->cursorPos == 0) && (encoderDirection < 0)){
    menuProperties->topIndex--;
    if(menuProperties->topIndex < 0) menuProperties->topIndex = 0;
  }
  else if((menuProperties->cursorPos == 1) && (encoderDirection > 0)){
    menuProperties->topIndex++;
    if(menuProperties->topIndex > (menuProperties->itemNumber - 2)) menuProperties->topIndex = (menuProperties->itemNumber - 2);
  }

  if((encoderDirection != 0) || (*firstFrame)){
    if(*firstFrame) *firstFrame = false;
    encoder.write(0);
    lcd.clear();
    lcd.setCursor(15, menuProperties->cursorPos);
    lcd.write((byte) arrowLeft);
    lcd.setCursor(0,0);
    lcd.print(menuItemNames[menuProperties->topIndex]);
    lcd.setCursor(0,1);
    lcd.print(menuItemNames[menuProperties->topIndex + 1]);
  }

  menuProperties->selectedIndex = menuProperties->topIndex + menuProperties->cursorPos;

  return 0;
}


/**
 * @brief Custom value selection menu on lcd display
 * 
 * @param valueName Name of value that will be selected
 * @param firstFrame First frame of FSM state flag
 * @param minValue Min. possible value
 * @param maxValue Max. possible value
 * @param selectedValue Currently selected value
 * @return uint8_t 0
 */
uint8_t lcdValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValue, uint8_t maxValue, int8_t &selectedValue){

  int8_t encoderDirection = encoder.read() / 4;

  if(minValue >= maxValue) selectedValue = maxValue;
  else{
    if(encoderDirection > 0) selectedValue++;
    else if(encoderDirection < 0) selectedValue --;

    if(selectedValue < minValue) selectedValue = minValue;
    else if(selectedValue > maxValue) selectedValue = maxValue;
  }

  if((encoderDirection != 0) || (*firstFrame)){
    if(*firstFrame) *firstFrame = false;
    encoder.write(0);
    lcd.clear();
    lcd.setCursor(15, 1);
    lcd.write((byte) arrowLeft);
    lcd.setCursor(0,0);
    lcd.print(valueName);
    lcd.setCursor(0,1);
    lcd.print(selectedValue);
  }

  return 0;
}


/**
 * @brief Custom floating point value selection menu on lcd display
 * 
 * @param valueName Name of value that will be selected
 * @param firstFrame First frame of FSM state flag
 * @param minValue Min. possible value
 * @param maxValue Max. possible value
 * @param selectedValue Currently selected value
 * @return uint8_t 0
 */
uint8_t lcdFloatValueMenu(const char* valueName, bool* firstFrame,  float minValue, float maxValue, float &selectedValue){

  int8_t encoderDirection = encoder.read() / 4;

  if(minValue >= maxValue) selectedValue = maxValue;
  else{
    if(encoderDirection > 0) selectedValue += 0.1f;
    else if(encoderDirection < 0) selectedValue -= 0.1f;

    if(selectedValue < minValue) selectedValue = minValue;
    else if(selectedValue > maxValue) selectedValue = maxValue;
  }

  if((encoderDirection != 0) || (*firstFrame)){
    if(*firstFrame) *firstFrame = false;
    encoder.write(0);
    lcd.clear();
    lcd.setCursor(15, 1);
    lcd.write((byte) arrowLeft);
    lcd.setCursor(0,0);
    lcd.print(valueName);
    lcd.setCursor(0,1);
    lcd.print(selectedValue);
    lcd.setCursor(3, 1);
    lcd.print(" ");
  }

  return 0;
}


/**
 * @brief Custom "Mäxle" dice value selection menu on lcd display
 * 
 * @param valueName Printed Text
 * @param firstFrame First frame of FSM state flag
 * @param minValueIndex Min. dice value
 * @param selectedValueIndex Currently slected dice value
 * @return uint8_t 0
 */
uint8_t lcdDiceValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValueIndex, int8_t &selectedValueIndex){

  uint8_t maxValueIndex = 20;       // valueToIndexLUT[21] = 20;

  int8_t encoderDirection = encoder.read() / 4;

  if(minValueIndex >= maxValueIndex) selectedValueIndex = maxValueIndex;
  else{
    if(encoderDirection > 0) selectedValueIndex++;
    else if(encoderDirection < 0) selectedValueIndex --;

    if(selectedValueIndex < minValueIndex) selectedValueIndex = minValueIndex;
    else if(selectedValueIndex > maxValueIndex) selectedValueIndex = maxValueIndex;
  }

  if((encoderDirection != 0) || (*firstFrame)){
    if(*firstFrame) *firstFrame = false;
    encoder.write(0);
    lcd.clear();
    lcd.setCursor(15, 1);
    lcd.write((byte) arrowLeft);
    lcd.setCursor(0,0);
    lcd.print(valueName);
    lcd.setCursor(0,1);
    lcd.print(indexToValueLUT[selectedValueIndex]);
  }

  return 0;
}