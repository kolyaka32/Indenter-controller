
#include <stdio.h>
#include <string.h>
#include "task.h"


// Variables
App app;
Motor motor = {.pinPul = MOTOR_STEP_Pin, .pinDir = MOTOR_REVERCE_Pin,
	.portPul = MOTOR_STEP_GPIO_Port, .portDir = MOTOR_REVERCE_GPIO_Port};


void init() {
	// Motor initialisation
	motorReset(&motor);

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
	app.counter = 0;
}

void loop() {
	// Check new USB instructions
	// Already procced asynchonous
	// Check new RS-485 messages
	//checkSensors();

	// Tact actions
	// RS485 Ask force sensor
	if (app.counter % 50 == 0) {
		askForceSensor();
	}
	// RS485 Ask temperature sensor
	if (app.counter % 200 == 10) {
		askTemperatureSensor();
	}

	// USB Send packet with last measured values
	if (app.counter % 50 == 20) {
		sendUSBpacket();
	}
	// USB Send packet with current state (also as alive)
	if (app.counter % 50 == 30) {
		sendUSBstate();
	}
	// Motor movement
	// ! //HAL_GPIO_WritePin(GPIOB, MOTOR_STEP_Pin, motor);
	motorCheckStep(&motor);
	// Check end conditionals
	// !

	// Display?
	//displaySetAddress(0);
	//displayPutString("Hello");

	// Show alive
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, app.counter%2);
	app.counter++;

	HAL_Delay(10);
}
