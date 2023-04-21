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

#endif