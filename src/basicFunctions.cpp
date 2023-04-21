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
#include <LiquidCrystal.h>
#include <Encoder.h>

#include "config.h"
#include "tcsFunctions.h"
#include "customLCDCharacters.h"

#include "basicFunctions.h"


static LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
static Encoder encoder(RE_CLK, RE_DT);


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

    lcd.begin(LCD_COLS, LCD_ROWS);
    createCustomLCDChars();

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


uint8_t createCustomLCDChars(){

  lcd.createChar(arrowLeft, arrowLeftChar);

  return 0;
}


uint8_t lcdSelection(const char** menuItems, uint8_t &selectedIndex){

  //static uint8_t menuItemNumber = menuItems.itemNumber;
  static uint8_t menuItemNumber = 3;
  static int32_t prevEncoderPos = -1;
  int32_t encoderPos = encoder.read() / 4;
  uint8_t firstLineIndex;
  

  if(encoderPos > (menuItemNumber - 2)) firstLineIndex = menuItemNumber - 2;
  else if (encoderPos < 0) firstLineIndex = 0;
  else firstLineIndex = encoderPos;

  if(encoderPos > (menuItemNumber - 1)) encoder.write((menuItemNumber - 1));
  else if (encoderPos < 0) encoder.write(0);
  
  /*Serial.print(menuItems.itemNames[selectedIndex]);
  Serial.print("\t");
  Serial.print(menuItems.itemNames[selectedIndex + 1]);
  Serial.print("\t");*/
  Serial.print(menuItemNumber);
  Serial.print("\t");
  Serial.print(encoderPos);
  Serial.print("\t");
  Serial.print(selectedIndex);
  Serial.print("\r\n");
  
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItems.itemNames[selectedIndex]);
  lcd.setCursor(0,1);
  lcd.print(menuItems.itemNames[selectedIndex + 1]);*/

  if(encoderPos != prevEncoderPos){
    prevEncoderPos = encoderPos;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(menuItems[firstLineIndex]);
    lcd.setCursor(0,1);
    lcd.print(menuItems[firstLineIndex + 1]);

    lcd.setCursor(15, 0);
    lcd.write((byte) arrowLeft);
  }

  return 0;
}