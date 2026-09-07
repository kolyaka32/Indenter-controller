#pragma once

#include "main.h"


// Data types
typedef float Position;
typedef float Force;
typedef uint16_t Temp;

extern Force lastForce;
extern Temp lastTemp;

void sensorInit();

// Function for interacting with sensors
void askForceSensor();
void askTemperatureSensor();

// Variants of getting data and updating it
void checkSensors();
