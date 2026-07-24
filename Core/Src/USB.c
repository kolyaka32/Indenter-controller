

#include <usbd_cdc_if.h>
#include "USB.h"
#include "task.h"


int statePacket[1];
typedef struct {
	int type;
	Position position;
	Force force;
	uint16_t temperature;
} DataPacket;
DataPacket dataPacket;
typedef struct {
	int type;
	int pos;
} PosPacket;
PosPacket posPacket;


void sendUSBstate() {
	// Check, what to send depend on state
	switch (state) {
	case Waiting:
		statePacket[0] = 1; // Message of waiting state
		CDC_Transmit_FS((uint8_t*)statePacket, sizeof(statePacket));
		break;

	case Working:
		statePacket[0] = 2; // Message of working state
		CDC_Transmit_FS((uint8_t*)statePacket, sizeof(statePacket));
		break;
	}
}

void sendUSBpacket() {
	dataPacket.type = 3;
	dataPacket.position = motorGetLocation(&motor);
	dataPacket.force = lastForce;
	dataPacket.temperature = lastTemp;
	CDC_Transmit_FS((uint8_t*)&dataPacket, sizeof(dataPacket));
}

void sendUSBpos() {
	posPacket.type = 4;
	posPacket.pos = motorGetPosition(&motor);
	CDC_Transmit_FS((uint8_t*)&posPacket, sizeof(posPacket));
}

void sendUSBreachPos() {
	statePacket[0] = 5;
	CDC_Transmit_FS((uint8_t*)statePacket, sizeof(statePacket));
}

void sendUSBreachForce() {
	statePacket[0] = 6;
	CDC_Transmit_FS((uint8_t*)statePacket, sizeof(statePacket));
}


void getUSBdata(uint8_t* _buf, uint32_t* _len) {
	// Deciding to do by first byte
	switch(_buf[0]) {
	case 0:
		// Code of none
		break;

	case 1:
		// Get position
		sendUSBpos();
		// Not changing state
		break;

	case 2:
		// Set reach force
		// Not changing state
		waiting = WaitReachForce;
		break;

	case 3:
		// Set lost force
		// Not changing state
		waiting = WaitLoseForce;
		break;

	case 4:
		// Set move up at work speed
		motorSetLowSpeed(&motor);
		motorSetMovePlus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case 5:
		// Set move down at work speed
		motorSetLowSpeed(&motor);
		motorSetMoveMinus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case 6:
		// Set move up at idle speed
		motorSetHighSpeed(&motor);
		motorSetMovePlus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case 7:
		// Set move down at idle speed
		motorSetHighSpeed(&motor);
		motorSetMoveMinus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case 8:
		// Set move up by number of steps
		motorSetLowSpeed(&motor);
		motorSetMovePlusBy(&motor, *((float*)(_buf+1)));
		state = Working;
		waiting = WaitPos;
		break;

	case 9:
		// Set move down by number of steps
		motorSetLowSpeed(&motor);
		motorSetMoveMinusBy(&motor, *((float*)(_buf+1)));
		state = Working;
		waiting = WaitPos;
		break;

	case 10:
		// Set move to target
		motorSetLowSpeed(&motor);
		motorSetTarget(&motor, *((int*)(_buf+1)));
		state = Working;
		waiting = WaitPos;
		break;

	case 11:
		// Code of stop movement
		motorSetStop(&motor);
		state = Waiting;
		waiting = WaitNone;
		break;

	default:
		break;
	}
}
