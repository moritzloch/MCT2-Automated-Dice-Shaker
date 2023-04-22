#ifndef MENU_FUNCTIONS
#define MENU_FUNCTIONS

#include <stdint.h>


uint8_t initMenu();

struct MenuProperties{
    bool firstFrame = true;
    uint8_t cursorPos = 0;
    uint8_t itemNumber;
    int8_t topIndex = 0;
    uint8_t selectedIndex = 0;
    //char* itemNames[];
};

static MenuProperties mainMenuProperties;

static const char* mainMenuItemNames[3] = {
    "Maexle",
    "Wuerfeln",
    "Einstellungen"
};


uint8_t createCustomLCDChars();

uint8_t resetMenuProperties(MenuProperties &menuProperties, uint8_t itemNumber);

void encoderPressed();

uint8_t lcdScrollMenu(MenuProperties &menuProperties, const char** menuItemNames);

#endif