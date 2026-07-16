#pragma once

#include "main.h"

typedef uint32_t timer;

typedef struct {
	timer nextPul;   // Time, when required next pull
	timer delta;
	// Pins
	uint16_t pinPul;
	GPIO_TypeDef* portPul;
	uint16_t pinDir;
	GPIO_TypeDef* portDir;
} Motor;

void motorReset(Motor* motor);
// Commands from upper level
void motorSetSpeed(Motor* motor, signed char speed);
void motorStop(Motor* motor);
// Every cycle update
void motorCheckStep(Motor* motor);
