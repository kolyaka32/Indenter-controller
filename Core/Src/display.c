
#include <stdio.h>
#include <string.h>
#include "display.h"


void displayWait(uint32_t _time) {
	// ?
	HAL_TIM_Base_Start(&htim1);
	TIM1->CNT = 0;
	while (TIM1->CNT < _time*1000) {}
	//HAL_Delay(1);
}

void displaySet(bool _A, unsigned char _data) {
	// Setting sending data
	HAL_GPIO_WritePin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin, 1);

	// Setting data
	HAL_GPIO_WritePin(DISPLAY_RW_GPIO_Port, DISPLAY_RW_Pin, 0);
	HAL_GPIO_WritePin(DISPLAY_A0_GPIO_Port, DISPLAY_A0_Pin,  _A & 0x01);
	HAL_GPIO_WritePin(DISPLAY_DB7_GPIO_Port, DISPLAY_DB7_Pin, _data&0x80);
	HAL_GPIO_WritePin(DISPLAY_DB6_GPIO_Port, DISPLAY_DB6_Pin, _data&0x40);
	HAL_GPIO_WritePin(DISPLAY_DB5_GPIO_Port, DISPLAY_DB5_Pin, _data&0x20);
	HAL_GPIO_WritePin(DISPLAY_DB4_GPIO_Port, DISPLAY_DB4_Pin, _data&0x10);
	HAL_GPIO_WritePin(DISPLAY_DB3_GPIO_Port, DISPLAY_DB3_Pin, _data&0x08);
	HAL_GPIO_WritePin(DISPLAY_DB2_GPIO_Port, DISPLAY_DB2_Pin, _data&0x04);
	HAL_GPIO_WritePin(DISPLAY_DB1_GPIO_Port, DISPLAY_DB1_Pin, _data&0x02);
	HAL_GPIO_WritePin(DISPLAY_DB0_GPIO_Port, DISPLAY_DB0_Pin, _data&0x01);

	displayWait(1);

	HAL_GPIO_WritePin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin, 0);
}

void displayInit() {
	// Resetting
	HAL_GPIO_WritePin(DISPLAY_XRES_GPIO_Port, DISPLAY_XRES_Pin, 1);
	displayWait(2);
	HAL_GPIO_WritePin(DISPLAY_XRES_GPIO_Port, DISPLAY_XRES_Pin, 1);
	// Waiting of start of display itself
	HAL_Delay(50);

	// Working in 8-bit mode
	// Setting interface bit depth
	displaySet(0, 0x3F);
	displayWait(40);

	displaySet(0, 0x3F);
	displayWait(40);
	// Single block start
	displaySet(0, 0x3F);
	displayWait(40);
	// Setting basic parameters
	displaySet(0, 0x38);
	displayWait(40);
	// Turning on display
	displaySet(0, 0x04);
	displayWait(40);
	// Clearing display
	displaySet(0, 0x0E);
	displayWait(40);
	// Setting input mode
	displaySet(0, 0x01);
	displayWait(40);
}

void displayReset() {

}

void displaySetMode(bool _cursor, bool _page) {
	displaySet(0, 0x1100 | (_cursor << 1) | _page);
	displayWait(10);
}

void displayClear() {
	displaySet(0, 0x01);
	HAL_Delay(2);
}

void displaySetAddress(unsigned char _offset) {
	displaySet(0, _offset + 0x80);
	displayWait(10);
}

void displayPutChar(unsigned char _symbol) {
	displaySet(1, _symbol);
	displayWait(10);
}

void displayPutString(const char* _text) {
	for (int i=0; (i < DISPLAY_LENGTH) && _text[i]; ++i) {
		displayPutChar(_text[i]);
	}
}

void displayPutFloat(float _value) {
	char buffer[7];
	snprintf(buffer, sizeof(buffer), "%2.2f", _value);

	for (char* c = buffer; *c; ++c) {
		displayPutChar(*c);
	}
}

void displayPutInt16(int16_t _value) {
	char buffer[7];
	snprintf(buffer, sizeof(buffer), "%+hd", _value);

	for (char* c = buffer; *c; ++c) {
		displayPutChar(*c);
	}
}

void displayBlit() {
	displaySet(0, 0x14);
	displayWait(10);
}
