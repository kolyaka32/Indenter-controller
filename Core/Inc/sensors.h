#pragma once

#include "main.h"


// Object for work with UART (rs485 in our case)
extern UART_HandleTypeDef huart2;


void sensorInit();

// Function for interacting with sensors
void askForceSensor();
void askTemperatureSensor();

// Variants of getting data and updating it
void checkSensors();
void recieveUART();
