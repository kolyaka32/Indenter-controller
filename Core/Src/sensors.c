#include "sensors.h"
#include "task.h"


typedef union {
	float force;
	uint8_t part[4];
} ForceUn;
typedef union {
	uint16_t temp;
	uint8_t part[2];
} TempUn;
static uint8_t buffer[9];
Force lastForce;
uint16_t lastTemp;
Force compareGreaterForce = 20.0;
Force compareLessForce = 10.0;

void sensorInit() {
	HAL_UART_Receive_IT(&huart2, buffer, sizeof(buffer));
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);
}

void askForceSensor() {
	// Set driver enable
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);
	// Sending request:
	// Get from 16 device multiple register (code 3) from 0x0046 0x0004 bytes, checksum:
	uint8_t data[] = {0x10, 0x03, 0x00, 0x46, 0x00, 0x04, 0xA6, 0x9D};
	HAL_UART_Transmit(&huart2, data, sizeof(data), 10);
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);
}

void askTemperatureSensor() {
	// Set driver enable
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);
	// From 1 device read (4) from 0x0000 1 register, checksum:
	uint8_t data[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB};
	HAL_UART_Transmit(&huart2, data, sizeof(data), 10);
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);
}

void checkSensors() {
	// Check, if not get full
	if (HAL_UART_Receive_IT(&huart2, buffer, sizeof(buffer)) == HAL_BUSY) {
		return;
	}

	// Find depend on source address (first byte)
	switch (buffer[0]) {
	case 16:
		// Force sensor
		ForceUn forceUn = {.part = {buffer[6], buffer[5], buffer[4], buffer[3]}};
		lastForce = forceUn.force;
		// Check on end waiting
		if (waiting == WaitReachForce) {
			if (lastForce > compareGreaterForce) {
				sendUSBreachPos();
				motorSetStop(&motor);
				state = Waiting;
				waiting = WaitNone;
			}
		} else if (waiting == WaitLoseForce) {
			if (lastForce < compareLessForce) {
				sendUSBreachPos();
				motorSetStop(&motor);
				state = Waiting;
				waiting = WaitNone;
			}
		}
		break;

	case 1:
		// Temperature sensor
		lastTemp = buffer[3]*256+buffer[4];
		break;

	default:
		break;
	};
	// USB Send packet with last measured values
	sendUSBpacket();
}

/*void checkSensorsBlocking() {
	if (HAL_UART_Receive(&huart2, buffer, sizeof(buffer), 100) == HAL_OK) {
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		// Find depend on source address (first byte)
		switch (buffer[0]) {
		case 16:
			// Force sensor
			lastForce = *(Force*)(buffer+3);
			// Check on end waiting
			if (waiting == WaitReachForce) {
				if (lastForce > compareGreaterForce) {
					sendUSBreachPos();
					motorSetStop(&motor);
					state = Waiting;
					waiting = WaitNone;
				}
			} else if (waiting == WaitLoseForce) {
				if (lastForce < compareLessForce) {
					sendUSBreachPos();
					motorSetStop(&motor);
					state = Waiting;
					waiting = WaitNone;
				}
			}
			break;

		case 1:
			// Temperature sensor
			lastTemp = *(Temp*)(buffer+3);
			break;
		};
		// USB Send packet with last measured values
		sendUSBpacket();
	}
}*/

/*void getSensorsShowDisplay() {
	uint8_t buffer[30];
	if (HAL_UART_Receive(&huart2, buffer, sizeof(buffer), 0) == HAL_OK) {
		// Find depend on source address (first byte)
		switch (buffer[0]) {
		case 16:
			// Force sensor
			// Set offset
			displaySetAddress(7);
			displayPutFloat(*(float*)(buffer+3));
			break;

		case 1:
			// Temperature sensor
			displaySetAddress(46);
			displayPutFloat(*(int16_t*)(buffer+3));
			break;
		};
	}
}*/

/*uint16_t checksum(char* data) {
	// Not using
	uint16_t sum = 0xffff;
	for (int i=0; i < sizeof(data); ++i) {
		sum ^= data[i];
		for (int j=0; j<8; j++) {
			if (sum & 1) {
				sum = (sum>>1) ^ 0xA001; // LSB(b0)=1
			} else {
				sum = sum>>1;
			}
		}
	}
	return sum;
}*/
