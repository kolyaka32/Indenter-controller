#pragma once

#include "USB.h"
#include "sensors.h"
#include "display.h"
#include "motor.h"


// States of current device
enum State {
	None,
	Waiting,
	Working,
};

// Posible waitings
enum Waiting {
	Undefined,
	WaitNone,
	WaitPos,
	WaitReachForce,
	WaitLoseForce,
};

// All current app data
extern Motor motor;
extern int counter;
extern int state;
extern int waiting;
extern float forceTarget;

// Function, that run before main cycle once
void init();

// Function, that execute every cycle of controller run
void loop();
