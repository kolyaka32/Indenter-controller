

#include <usbd_cdc_if.h>
#include "USB.h"
#include "task.h"


typedef uint16_t Type;
typedef struct {
	Type type;
} StatePacket;
StatePacket statePacket;
typedef struct {
	Type type;
	uint16_t unused;
	Position position;
	Force force;
	uint16_t temperature;
} DataPacket;
DataPacket dataPacket;
typedef struct {
	Type type;
	uint16_t speed;
	uint16_t pos;
} PosPacket;
PosPacket posPacket;
typedef struct {
	Type type;
	uint16_t speed;
	float distance;
} StepPacket;
typedef struct {
	Type type;
	uint16_t speed;
} MovePacket;
typedef struct {
	Type type;
	uint16_t unused;
	float force;
} ForcePacket;


enum SendPacketType {
	SendNone,        // Nothing, for apply connection
	// Current state
	SendWaiting,     // Currently waiting for command
	SendWorking,     // Currently moving
	// New measures
	SendPacket,      // One frame with new [position], [force] and [temperature]
	// Complete task
	SendPosition,    // Signal with current [position]
	SendReachPos,    // Signal, that reach setted position
	SendReachForce,  // Signal, that reach setted force
};

// Send part
void sendUSBstate() {
	// Check, what to send depend on state
	switch (state) {
	case Waiting:
		statePacket.type = SendWaiting;
		CDC_Transmit_FS((uint8_t*)&statePacket, sizeof(statePacket));
		break;

	case Working:
		statePacket.type = SendWorking;
		CDC_Transmit_FS((uint8_t*)&statePacket, sizeof(statePacket));
		break;
	}
}

void sendUSBpacket() {
	dataPacket.type = SendPacket;
	dataPacket.position = motorGetLocation(&motor);
	dataPacket.force = lastForce;
	dataPacket.temperature = lastTemp;
	CDC_Transmit_FS((uint8_t*)&dataPacket, sizeof(dataPacket));
}

void sendUSBpos() {
	posPacket.type = SendPosition;
	posPacket.speed = 0;
	posPacket.pos = motorGetPosition(&motor);
	CDC_Transmit_FS((uint8_t*)&posPacket, sizeof(posPacket));
}

void sendUSBreachPos() {
	statePacket.type = SendReachPos;
	CDC_Transmit_FS((uint8_t*)&statePacket, sizeof(statePacket));
}

void sendUSBreachForce() {
	statePacket.type = SendReachForce;
	CDC_Transmit_FS((uint8_t*)&statePacket, sizeof(statePacket));
}


// Recieve part
enum GetPacketType {
	GetNone,
	// Getters
	GetPos,       // Ask current position
	// Set sensors
	GetReachForce,   // Send to said, when exceed setted [force]
	GetLowerForce,   // Send to send, when get lower setted [force]
	// Movement
	GetSetMoveUp,    // Start move up with [speed]
	GetSetMoveDown,  // Start move down with [speed]
	GetSetStepUp,    // Move up with [speed] by specified number of [steps]
	GetSetStepDown,  // Move down with [speed] by specified number of [steps]
	GetSetMoveTo,    // Move with [speed] to setted [position]
	// Stop
	GetSetStop,      // Stop movement
};

void getUSBdata(uint8_t* _buf, uint32_t* _len) {
	// Deciding to do by first byte
	switch(((StatePacket*)_buf)->type) {
	case GetNone:
		// Code of none
		break;

	case GetPos:
		// Get position
		sendUSBpos();
		// Not changing state
		break;

	case GetReachForce:
		// Set reach force
		// Not changing state
		waiting = WaitReachForce;
		forceTarget = ((ForcePacket*)_buf)->force;
		break;

	case GetLowerForce:
		// Set lost force
		// Not changing state
		waiting = WaitLoseForce;
		forceTarget = ((ForcePacket*)_buf)->force;
		break;

	case GetSetMoveUp:
		// Set move up
		motorSetSpeedPacket(&motor, ((MovePacket*)_buf)->speed);
		motorSetMovePlus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case GetSetMoveDown:
		// Set move down at speed selected speed
		motorSetSpeedPacket(&motor, ((MovePacket*)_buf)->speed);
		motorSetMoveMinus(&motor);
		state = Working;
		waiting = WaitNone;
		break;

	case GetSetStepUp:
		// Set move up by number of steps at speed selected speed
		motorSetSpeedPacket(&motor, ((StepPacket*)_buf)->speed);
		motorSetMovePlusBy(&motor, ((StepPacket*)_buf)->distance);
		state = Working;
		waiting = WaitPos;
		break;

	case GetSetStepDown:
		// Set move down by number of steps at speed selected speed
		motorSetSpeedPacket(&motor, ((StepPacket*)_buf)->speed);
		motorSetMoveMinusBy(&motor, ((StepPacket*)_buf)->distance);
		state = Working;
		waiting = WaitPos;
		break;

	case GetSetMoveTo:
		// Set move to target
		motorSetSpeedPacket(&motor, ((PosPacket*)_buf)->speed);
		motorSetTarget(&motor, ((PosPacket*)_buf)->pos);
		state = Working;
		waiting = WaitPos;
		break;

	case GetSetStop:
		// Code of stop movement
		motorSetStop(&motor);
		state = Waiting;
		waiting = WaitNone;
		break;

	default:
		break;
	}
}
