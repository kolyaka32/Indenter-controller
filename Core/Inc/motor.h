#pragma once

#include "main.h"


typedef uint32_t timer;

typedef struct {
	//timer delta;        // Calculated time between pulses for setted speed
	int currentPos;     // Current position, in substeps
	int targetPos;      // Target position where motor try to get, in substeps
	int8_t direction;   // +1 for going plus and -1 for going minus
	// Gear ratios
	float gearboxRatio;   // Rotations of motor per one mm
	float substepsCount;  // Count of substeps in step
	// Connection
	uint16_t pinPul;
	GPIO_TypeDef* portPul;
	uint16_t pinDir;
	GPIO_TypeDef* portDir;
	//uint16_t pinEna;
	//GPIO_TypeDef* portEna;
	TIM_HandleTypeDef *timHandler;
} Motor;

void motorReset(Motor* motor);
// Commands from upper level
void motorSetMovePlusBy(Motor* motor, float value);   // Set movement in plus direction [mm]
void motorSetMoveMinusBy(Motor* motor, float value);  // Set movement in minus direction [mm]
void motorSetMovePlus(Motor* motor);                  // Set move in plus direction
void motorSetMoveMinus(Motor* motor);                 // Set move in minus direction
void motorSetTarget(Motor* motor, int target);        // Set movement to target position [steps]
void motorSetSpeed(Motor* motor, float speed);        // Set speed [mm/s]
void motorSetHighSpeed(Motor* motor);                 // Set movement speed to high (idle movement)
void motorSetLowSpeed(Motor* motor);                  // Set movement speed to low (work movement)
void motorSetStop(Motor* motor);                      // Stop moving anywhere
int motorGetPosition(const Motor* motor);             // Return current position [steps]
float motorGetLocation(const Motor* motor);           // Return current position [mm]
// Interrupts
void motorCheckStep(Motor* motor);
// Interaction with sensors
//void motorReachStart(Motor* motor);
//void motorReachEnd(Motor* motor);
