#include "sensors.h"
#include "task.h"


void sensorInit() {
	uint8_t buffer[40];
	HAL_UART_Receive_IT(&huart2, buffer, sizeof(buffer));
}

void askForceSensor() {
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);
	// Sending request:
	// Get from 16 device multiple register (code 3) from 0x0046 0x0004 bytes, checksum:
	uint8_t data[] = {0x10, 0x03, 0x00, 0x46, 0x00, 0x04, 0xA6, 0x9D};
	HAL_UART_Transmit(&huart2, data, sizeof(data), 10);
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);
}

void askTemperatureSensor() {
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);
	// From 1 device read (4) from 0x0000 1 register, checksum:
	uint8_t data[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA};
	HAL_UART_Transmit(&huart2, data, sizeof(data), 10);
	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);
}

void checkSensors() {
	uint8_t buffer[100];
	if (HAL_UART_Receive_IT(&huart2, buffer, sizeof(buffer)) == HAL_OK) {
		// Find depend on source address (first byte)
		switch (buffer[0]) {
		case 16:
			// Force sensor
			app.lastForce = *(Force*)(buffer+3);
			break;

		case 1:
			// Temperature sensor
			app.lastTemperature = *(Temp*)(buffer+3);
			break;
		};
	}
}

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
