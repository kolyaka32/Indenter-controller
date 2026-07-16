
#pragma once

#include "main.h"


// Constants
#define DISPLAY_LENGTH 16

typedef char bool;  // True or false

// Special commands to work with display
void displayWait(uint32_t time);
void displaySet(bool A0, unsigned char data);

// Working with display
void displayInit();
//void displayReset();

void displaySetMode(bool cursor, bool page);
void displayClear();
void displaySetAddress(unsigned char offset);
void displayPutChar(unsigned char symbol);
void displayPutString(const char* text);
void displayPutFloat(float value);
void displayPutInt16(int16_t value);
void displayBlit();
