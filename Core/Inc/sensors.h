#pragma once

#include "main.h"


// Data types
typedef float Position;
typedef float Force;
typedef uint16_t Temp;

extern Force lastForce;
extern Temp lastTemp;

// Forces for comparison in waiting [N]
extern Force compareGreaterForce;
extern Force compareLessForce;

void sensorInit();

// Function for interacting with sensors
void askForceSensor();
void askTemperatureSensor();

// Variants of getting data and updating it
void checkSensors();
