#pragma once

#include "USB.h"
#include "sensors.h"
#include "display.h"
#include "motor.h"


// Data types
typedef float Force;
typedef float Temp;

// Last measured values
typedef struct {
	int counter;
	Force lastForce;
	Temp lastTemperature;
} App;

// All current app data
extern App app;
extern Motor motor;

// Function, that run before main cycle once
void init();

// Function, that execute every cycle of controller run
void loop();
