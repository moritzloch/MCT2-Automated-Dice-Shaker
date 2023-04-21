#ifndef BASIC_FUNCTIONS
#define BASIC_FUNCTIONS

#include <stdint.h>


/*class MenuItems{
    public:
        uint8_t itemNumber;
        char** itemNames;
        MenuItems(const char** itemNamesInput, uint8_t itemNumberInput){
            itemNumber = itemNumberInput;
            for(uint8_t i = 0; i < itemNumber; i++){
                strcpy(itemNames[i], itemNamesInput[i]);
            }
        }
};

static const char* mainMenuItemNames[3] = {
    "Maexle",
    "Wuerfeln",
    "Einstellungen"
};

static MenuItems mainMenuItems(mainMenuItemNames, (uint8_t) 3);*/


uint8_t initArduino();

uint8_t setRGB(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);

char getChar();

uint8_t clearInput();

uint8_t createCustomLCDChars();

//uint8_t lcdSelection(MenuItems menuItems, uint8_t selectedIndex);

#endif