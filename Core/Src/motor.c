#include "motor.h"


void motorReset(Motor* _motor) {
	// Enabling the driver
	HAL_GPIO_WritePin(_motor->portPul, _motor->pinPul, 0);
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
	// Resetting values
	_motor->delta = 10;    // Will be at next tick
	_motor->nextPul = -1;  // Much of time
}

void motorSetSpeed(Motor* _motor, signed char _speed) {
	if (_speed == 0) {
		return;
	}
	if (_speed < 0) {
		// Reverse
		HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
		_motor->delta = 10;  // !
	} else {
		// Straight
		HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 0);
		_motor->delta = 10;  // !
	}
	_motor->nextPul = HAL_GetTick() + _motor->delta;
}

void motorStop(Motor* _motor) {
	_motor->nextPul = -1;  // Much of time, never happen
}

void motorCheckStep(Motor* _motor) {
	if (HAL_GetTick() > _motor->nextPul) {
		// Applying pull
		//HAL_GPIO_WritePin(GPIOB, _motor->pinPul, state);
		HAL_GPIO_TogglePin(_motor->portPul, _motor->pinPul);
		// Updating time
		_motor->nextPul = HAL_GetTick() + _motor->delta;
	}
}

int motorGetState(const Motor* _motor) {
	return 0;  // ! Ask, if required
}
