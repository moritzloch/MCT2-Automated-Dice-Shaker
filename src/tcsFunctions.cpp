/**
 * @file TCSFunctions.cpp
 * @author Marco Schweizer, Moritz Loch
 * @brief Functions for TCS3200 colour sensor
 * @version 1.0
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <MD_TCS230.h>

#include "config.h"
#include "basicFunctions.h"

#include "tcsFunctions.h"


/**
 * @brief Finite State Machine for reading RGB values from TCS3200 color sensor
 * 
 * @param state Current FSM state
 * @param readType Type of read (black calibration, white calibration, RGB)
 * @param rgb rgbValues of RGB read
 * @return uint8_t Next FSM state
 */
uint8_t fsmReadValue(uint8_t state, uint8_t readType, colorData rgb){

	switch(state){
        case 0:	//prompt for the user to start
            Serial.print(F("\n\nReading value for "));
            switch(readType){
                case READ_BLACK_CALIB:	
                    Serial.print(F("BLACK calibration"));	
                    break;

                case READ_WHITE_CALIB:	
                    Serial.print(F("WHITE calibration"));	
                    break;

                case READ_RGB:	        
                    Serial.print(F("DATA"));				
                    break;

                default:		        
                    Serial.print(F("Unclear Read-Data-Type"));					
                    break;
            }
            
            Serial.print(F("\nPress any key to start ..."));
            state++;
            break;

        case 1:	//wait for user input
            getChar();
            clearInput();
            state++;
            break;

        case 2:	//start the reading process
            TCS.read();
            state++;
            break;

        case 3:	//wait for read to complete
            if (TCS.available()){

                sensorData	rawSensorData;

                switch(readType){
                    case READ_BLACK_CALIB:	
                        TCS.getRaw(&rawSensorData);	
                        TCS.setDarkCal(&rawSensorData);		
                        break;

                    case READ_WHITE_CALIB:	
                        TCS.getRaw(&rawSensorData);	
                        TCS.setWhiteCal(&rawSensorData);	
                        break;

                    case READ_RGB:	
                        TCS.getRGB(&rgb);

                        Serial.print(F("\nRGB is ["));
                        Serial.print(rgb.value[TCS230_RGB_R]);
                        Serial.print(F(","));
                        Serial.print(rgb.value[TCS230_RGB_G]);
                        Serial.print(F(","));
                        Serial.print(rgb.value[TCS230_RGB_B]);
                        Serial.print(F("]"));

                        setRGB(rgb.value[TCS230_RGB_R], rgb.value[TCS230_RGB_G], rgb.value[TCS230_RGB_B]);

                        break;
                }
                state++;
            }
            break;

        default:	// reset FSM
            state = 0;
            break;
	}

	return(state);
}