#ifndef MENU_FUNCTIONS
#define MENU_FUNCTIONS

#include <stdint.h>


uint8_t initMenu();

struct MenuProperties{
    uint8_t cursorPos = 0;
    uint8_t itemNumber;
    int8_t topIndex = 0;
    uint8_t selectedIndex = 0;
    //char* itemNames[];
};

static MenuProperties mainMenuProperties;

static const char* mainMenuItemNames[] = {
    "Maexle",
    "Wuerfeln",
    "Einstellungen"
};

static MenuProperties gameSettingsMenuProperties;

static const char* gameSettingsMenuItemNames[] = {
    "SPIEL BEGINNEN",
    "Anzahl Spieler",
    "Anzahl Leben",
    "Anzahl Wuerfel"
};

static MenuProperties lieDetectionMenuProperties;

static const char* lieDetectionMenuItemNames[] = {
    "Wahrheit",
    "Luege"
};


static MenuProperties* menus[] = {
    &mainMenuProperties,
    &gameSettingsMenuProperties,
    &lieDetectionMenuProperties
};
enum menuIndex_t{
    MENU_MAIN,
    MENU_GAMESETTINGS,
    MENU_LIEDETECTION
};


uint8_t lcdPrint(const char* firstLineText, const char* secondLineText, bool* firstFrame);

uint8_t lcdPrintPlayerNumber(const char* firstLineText, const char* secondLineText, uint8_t playerNumber, bool* firstFrame);

uint8_t lcdPrintDiceNumber(uint8_t diceNumber, bool* firstFrame);

uint8_t lcdPrintLives(uint8_t playerNumber, uint8_t lives, bool* firstFrame);

uint8_t createCustomLCDChars();

uint8_t resetMenuProperties(MenuProperties* menuProperties, uint8_t itemNumber);

uint8_t resetEncoder();

uint8_t lcdScrollMenu(MenuProperties* menuProperties, const char** menuItemNames, bool* firstFrame);

uint8_t lcdValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValue, uint8_t maxValue, int8_t &selectedValue);

uint8_t lcdDiceValueMenu(const char* valueName, bool* firstFrame,  uint8_t minValueIndex, int8_t &selectedValueIndex);

#endif