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
#include "customLCDCharacters.h"

#include "menuFunctions.h"


static LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
static Encoder encoder(RE_CLK, RE_DT);


uint8_t initMenu(){

  lcd.begin(LCD_COLS, LCD_ROWS);
  createCustomLCDChars();

  attachInterrupt(digitalPinToInterrupt(RE_SW), encoderPressed, FALLING);

  return 0;
}


uint8_t createCustomLCDChars(){

  lcd.createChar(arrowLeft, arrowLeftChar);

  return 0;
}


uint8_t resetMenuProperties(MenuProperties &menuProperties, uint8_t itemNumber){
  menuProperties.firstFrame = true;
  menuProperties.cursorPos = 0;
  menuProperties.itemNumber = itemNumber;
  menuProperties.topIndex = 0;
  menuProperties.selectedIndex = 0;

  return 0;
}


void encoderPressed(){
  Serial.println("State Transition");
}


uint8_t lcdScrollMenu(MenuProperties &menuProperties, const char** menuItemNames){

  int8_t encoderDirection = encoder.read() / 4;

  if(((menuProperties.cursorPos == 0) && (encoderDirection > 0)) || ((menuProperties.cursorPos == 1) && (encoderDirection < 0))){
    menuProperties.cursorPos ^= 1;
  }
  else if((menuProperties.cursorPos == 0) && (encoderDirection < 0)){
    menuProperties.topIndex--;
    if(menuProperties.topIndex < 0) menuProperties.topIndex = 0;
  }
  else if((menuProperties.cursorPos == 1) && (encoderDirection > 0)){
    menuProperties.topIndex++;
    if(menuProperties.topIndex > (menuProperties.itemNumber - 2)) menuProperties.topIndex = (menuProperties.itemNumber - 2);
  }

  if((encoderDirection != 0) || (menuProperties.firstFrame)){
    if(menuProperties.firstFrame) menuProperties.firstFrame = false;
    encoder.write(0);
    lcd.clear();
    lcd.setCursor(15, menuProperties.cursorPos);
    lcd.write((byte) arrowLeft);
    lcd.setCursor(0,0);
    lcd.print(menuItemNames[menuProperties.topIndex]);
    lcd.setCursor(0,1);
    lcd.print(menuItemNames[menuProperties.topIndex + 1]);
  }

  return 0;
}