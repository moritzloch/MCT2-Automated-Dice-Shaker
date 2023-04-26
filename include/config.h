#ifndef CONFIG
#define CONFIG
/**
 * @file config.h
 * @author Marco Schweizer, Moritz Loch
 * @brief Configuration header to specify pins and parameters
 * @version 1.0
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#define STANDARDNUMBEROFLIVES   3 
#define STANDARDNUMBEROFPLAYERS 2

#define STANDARDNUMBEROFDICE    1
#define STANDARDNUMBEROFEYES    6


#define LCD_COLS    16
#define LCD_ROWS     2

/**
 * @brief Defining input and output pins
 * 
 */
//Color Sensor TCS3200 pins
#define TCS_S2  12
#define TCS_S3  13
#define TCS_0E   8             //LOW = ENABLED
#define TCS_OUT 47             //Automatically selected for Arduino Mega by FreqCount.h
//RGB LED Output pins
#define RGB_RED   2   
#define RGB_BLUE  3
#define RGB_GREEN 4   
//LCD-Display pins
#define LCD_RS  52    
#define LCD_EN  50 
#define LCD_D4  48
#define LCD_D5  46 
#define LCD_D6  44
#define LCD_D7  42 
//Rotary Encoder pins
#define RE_SW   21
#define RE_DT   20
#define RE_CLK  19
//Reset pin
#define RESET   18

#endif