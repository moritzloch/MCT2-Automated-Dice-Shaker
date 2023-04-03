#ifndef TCS_FUNCTIONS
#define TCS_FUNCTIONS

#include <stdint.h>
#include <MD_TCS230.h>

#include "config.h"


/**
 * @brief instance of TCS3200 color sensor
 * 
 * @return MD_TCS230 instance TCS
 */
static MD_TCS230 TCS(TCS_S2, TCS_S3, TCS_0E);

/**
 * @brief readTypes for Read-FSM
 * 
 */
#define READ_BLACK_CALIB    0
#define READ_WHITE_CALIB    1
#define READ_RGB            2

/**
 * @brief calibration data
 * 
 */
static sensorData blackCalibData = {0, 0, 0};
static sensorData whiteCalibData = {0, 0, 0};

/**
 * @brief Color Table for matching
 * 
 */
typedef struct{
  char name[9];     // color name
  colorData rgb;    // RGB value
}colorTable;

static colorTable colorDescriptions[] = {
  {"WHITE",     {0, 0, 0}},
  {"BLACK",     {0, 0, 0}},
  {"YELLOW",    {0, 0, 0}},
  {"ORANGE",    {0, 0, 0}},
  {"RED",       {0, 0, 0}},
  {"GREEN",     {0, 0, 0}},
  {"BLUE",      {0, 0, 0}},
  {"PURPLE",    {0, 0, 0}},
};


uint8_t fsmReadValue(uint8_t state, uint8_t readType, colorData rgb);

uint8_t colorMatch(colorData rgb, uint8_t &matchedColorIndex);

#endif