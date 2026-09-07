
#include <stdio.h>
#include <string.h>
#include "task.h"


// Variables
Motor motor = {.gearboxRatio = 1.0/40.0, .substepsCount = 40000.0, .timHandler = &htim3,
	.pinPul = MOTOR_STEP_Pin, .pinDir = MOTOR_REVERCE_Pin,
	.portPul = MOTOR_STEP_GPIO_Port, .portDir = MOTOR_REVERCE_GPIO_Port};
int counter = 0;
int state = None;
int waiting = Undefined;
float forceTarget = 50.0;


void init() {
	// Motor initialisation
	motorReset(&motor);
	sensorInit();

	// Starting display
    displayInit();

	// Working with display
	/*displaySetAddress(1);
	displayWriteData(3);
	displayWriteData(3);
	displayWriteData(3);
	displayBlit();*/
	displaySetMode(0, 1);
	// Ручной режим
	displayPutString("Manual mode");

	// Signaling of correct initialisation
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
	counter = 0;
	state = Waiting;
	waiting = WaitNone;
}

void loop() {
	// USB Check new instructions
	// Already procced asynchonous

	// RS-485 Check new messages
	// Already procced asynchonous

	// RS485 Ask force sensor
	if (counter % 20 == 10) {
		askForceSensor();
	}
	// RS485 Ask temperature sensor
	if (counter % 50 == 15) {
		askTemperatureSensor();
	}

	// USB Send packet with current state (also as alive)
	if (counter % 200 == 5) {
		sendUSBstate();
	}
	// Motor movement - asynchronos

	// Display?
	//displaySetAddress(0);
	//displayPutString("Hello");

	// Show alive
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	counter++;

	HAL_Delay(10);
}
