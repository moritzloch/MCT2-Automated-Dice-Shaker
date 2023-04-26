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


static LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
static Encoder encoder(RE_CLK, RE_DT);


uint8_t initMenu(){

  lcd.begin(LCD_COLS, LCD_ROWS);
  createCustomLCDChars();

  return 0;
}


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

uint8_t lcdPrintDiceNumber(uint8_t diceNumber, bool* firstFrame){

  if(*firstFrame){
    *firstFrame = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Meine Zahl:");
    lcd.setCursor(0, 1);
    lcd.print(diceNumber);
  }

  return 0;
}


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
      lcd.setCursor(6, 1);
      lcd.print(" Leben");
    }
    else{
      lcd.setCursor(0, 1);
      lcd.print("keine Leben mehr");
    }

  }

  return 0;
}


uint8_t createCustomLCDChars(){

  lcd.createChar(arrowLeft, arrowLeftChar);

  return 0;
}


uint8_t resetMenuProperties(MenuProperties* menuProperties, uint8_t itemNumber){
  menuProperties->cursorPos = 0;
  menuProperties->itemNumber = itemNumber;
  menuProperties->topIndex = 0;
  menuProperties->selectedIndex = 0;

  return 0;
}


uint8_t resetEncoder(){
  encoder.write(0);
  return 0;
}


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


uint8_t lcdValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValue, uint8_t maxValue, int8_t &selectedValue){

  int8_t encoderDirection = encoder.read() / 4;

  if(encoderDirection > 0) selectedValue++;
  else if(encoderDirection < 0) selectedValue --;

  if(selectedValue < minValue) selectedValue = minValue;
  else if(selectedValue > maxValue) selectedValue = maxValue;

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


uint8_t lcdDiceValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValueIndex, int8_t &selectedValueIndex){

  uint8_t maxValueIndex = 20;       // valueToIndexLUT[21] = 20;

  int8_t encoderDirection = encoder.read() / 4;

  if(encoderDirection > 0) selectedValueIndex++;
  else if(encoderDirection < 0) selectedValueIndex --;

  if(selectedValueIndex < minValueIndex) selectedValueIndex = minValueIndex;
  else if(selectedValueIndex > maxValueIndex) selectedValueIndex = maxValueIndex;

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