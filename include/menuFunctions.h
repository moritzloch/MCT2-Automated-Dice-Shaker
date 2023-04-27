#ifndef MENU_FUNCTIONS
#define MENU_FUNCTIONS
/**
 * @file menuFunctions.h
 * @author Marco Schweizer, Moritz Loch
 * @brief Functions for LCD menus & printing
 * @version 1.0
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>


uint8_t initMenu();

/**
 * @brief Stores all menu properties
 * 
 */
struct MenuProperties{
    uint8_t cursorPos = 0;
    uint8_t itemNumber;
    int8_t topIndex = 0;
    uint8_t selectedIndex = 0;
    //char* itemNames[];
};

/**
 * @brief Main Menu for game mode selection
 * 
 */
static MenuProperties mainMenuProperties;
static const char* mainMenuItemNames[] = {
    "Maexle",
    "Wuerfeln",
    "Noten wuerfeln",
    "Einstellungen"
};

/**
 * @brief Menu for "Mäxle" game settings
 * 
 */
static MenuProperties gameSettingsMenuProperties;
static const char* gameSettingsMenuItemNames[] = {
    "SPIEL BEGINNEN",
    "Anzahl Spieler",
    "Anzahl Leben"
};

/**
 * @brief Menu for Lie Detection
 * 
 */
static MenuProperties lieDetectionMenuProperties;
static const char* lieDetectionMenuItemNames[] = {
    "Wahrheit",
    "Luege"
};

/**
 * @brief Menu for dice roll game mode settings
 * 
 */
static MenuProperties diceRollMenuProperties;
static const char* diceRollMenuItemNames[] = {
    "WUERFELN",
    "Anzahl Wuerfel",
    "Max. Augenzahl"
};

/**
 * @brief Menu for grade game mode settings
 * 
 */
static MenuProperties gradeMenuProperties;
static const char* gradeMenuItemNames[] = {
    "NOTEN WUERFELN",
    "Max. Bestnote"
};

/**
 * @brief Menu for general settings
 * 
 */
static MenuProperties settingsMenuProperties;
static const char* settingsMenuItemNames[] = {
    "ZURUECK",
    "Autom. Wuerfeln"
};

static MenuProperties autoDiceRollMenuProperties;
static const char* autoDiceRollMenuItemNames[] = {
    "Aus",
    "An"
};

/**
 * @brief Array of all menu properties
 * 
 */
static MenuProperties* menus[] = {
    &mainMenuProperties,
    &gameSettingsMenuProperties,
    &lieDetectionMenuProperties,
    &diceRollMenuProperties,
    &gradeMenuProperties,
    &settingsMenuProperties,
    &autoDiceRollMenuProperties
};
/**
 * @brief Enum menus
 * 
 */
enum menuIndex_t{
    MENU_MAIN,
    MENU_GAMESETTINGS,
    MENU_LIEDETECTION,
    MENU_DICEROLL,
    MENU_GRADE,
    MENU_SETTINGS,
    MENU_AUTODICEROLL
};


uint8_t lcdPrint(const char* firstLineText, const char* secondLineText, bool* firstFrame);

uint8_t lcdPrintPlayerNumber(const char* firstLineText, const char* secondLineText, uint8_t playerNumber, bool* firstFrame);

uint8_t lcdPrintMaexleDiceNumber(uint8_t diceNumber, bool* firstFrame);

uint8_t lcdPrintCustomDiceNumber(uint8_t nrDice, uint8_t nrEyes, bool* firstFrame);

uint8_t lcdPrintLives(uint8_t playerNumber, uint8_t lives, bool* firstFrame);

uint8_t lcdPrintGrade(float maxGrade, bool* firstFrame);

uint8_t createCustomLCDChars();

uint8_t resetMenuProperties(MenuProperties* menuProperties, uint8_t itemNumber);

uint8_t resetEncoder();

uint8_t lcdScrollMenu(MenuProperties* menuProperties, const char** menuItemNames, bool* firstFrame);

uint8_t lcdValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValue, uint8_t maxValue, int8_t &selectedValue);

uint8_t lcdFloatValueMenu(const char* valueName, bool* firstFrame,  float minValue, float maxValue, float &selectedValue);

uint8_t lcdDiceValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValueIndex, int8_t &selectedValueIndex);

#endif