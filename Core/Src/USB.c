

#include <usbd_cdc_if.h>
#include "USB.h"
#include "task.h"


void sendUSBstate() {
	char letter = 1;  // Message 1
	// !
	CDC_Transmit_FS((uint8_t*)&letter, sizeof(letter));
}

void sendUSBpacket() {
	// Sending packet by USB
	CDC_Transmit_FS((uint8_t*)&app, sizeof(app));
}

void getUSBdata(uint8_t* _buf, uint32_t* _len) {
	// Deciding to do by first byte
	switch(_buf[0]) {
	case 0:
		// Code of none
		break;

	case 1:
		// Code of stop movement
		motorStop(&motor);
		break;

	case 2:
		// Code of set main speed
		motorSetSpeed(&motor, _buf[1]);
		break;

	default:
		break;
	}
}
