#include "motor.h"
#include "task.h"


void motorReset(Motor* _motor) {
	// Enabling the driver
	//HAL_GPIO_WritePin(_motor->portEna, _motor->pinEna, 1);
	// Resetting direction
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
	HAL_GPIO_WritePin(_motor->portPul, _motor->pinPul, 1);
	// Resetting values
	_motor->currentPos = 0; // Not known
	_motor->targetPos = 0;  // Not moving anywhere
	_motor->direction = 1;  // Will move in positive direction
}

void motorSetMovePlusBy(Motor* _motor, float _delta) {
	// Create new target pos
	_motor->targetPos = _motor->currentPos;
	_motor->targetPos += _delta * 2.0 * _motor->substepsCount * _motor->gearboxRatio;
	// Set direction - straight
	_motor->direction = 1;
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 0);
	// Starting movement
	HAL_TIM_Base_Start_IT(_motor->timHandler);
}

void motorSetMoveMinusBy(Motor* _motor, float _delta) {
	// Create new target pos
	_motor->targetPos = _motor->currentPos;
	_motor->targetPos -= _delta * 2.0 * _motor->substepsCount * _motor->gearboxRatio;
	// Set direction - opposite
	_motor->direction = -1;
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
	// Starting movement
	HAL_TIM_Base_Start_IT(_motor->timHandler);
}

void motorSetMovePlus(Motor* _motor) {
	// Create new target - in opposite direction - nearly infinite
	_motor->targetPos = _motor->currentPos;
	// Set direction - straight
	_motor->direction = 1;
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 0);
	// Starting movement
	HAL_TIM_Base_Start_IT(_motor->timHandler);
}

void motorSetMoveMinus(Motor* _motor) {
	// Create new target - in opposite direction - nearly infinite
	_motor->targetPos = _motor->currentPos;
	// Set direction - opposite
	_motor->direction = -1;
	HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
	// Starting movement
	HAL_TIM_Base_Start_IT(_motor->timHandler);
}

void motorSetTarget(Motor* _motor, int _target) {
	// Set new target
	_motor->targetPos = _target;
	// Check direction
	if (_motor->targetPos > _motor->currentPos) {
		_motor->direction = 1;
		HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 0);
		// Starting movement
		HAL_TIM_Base_Start_IT(_motor->timHandler);
	} else {
		_motor->direction = -1;
		HAL_GPIO_WritePin(_motor->portDir, _motor->pinDir, 1);
		// Starting movement
		HAL_TIM_Base_Start_IT(_motor->timHandler);
	}
}

void motorSetSpeed(Motor* _motor, float _speed) {
	// Source freq - 96 MGz, Prescaler - 48 (47 + 1)
	// Setted, that counter equal work frequency
	int frequency = 1000000 / _speed / _motor->substepsCount / _motor->gearboxRatio;

	// Required to set ratio - 1 for correct functioning
	_motor->timHandler->Instance->CNT = 0;
	_motor->timHandler->Instance->ARR = frequency - 1;
}

void motorSetHighSpeed(Motor* _motor) {
	// Fast speed in 8000 steps/s or 4 mkm/s
	motorSetSpeed(_motor, 4.0);
}

void motorSetLowSpeed(Motor* _motor) {
	// Basic speed in 2000 steps/s or 1 mkm/s
	motorSetSpeed(_motor, 1.0);
}

void motorSetStop(Motor* _motor) {
	HAL_TIM_Base_Stop_IT(_motor->timHandler);
}

void motorCheckStep(Motor* _motor) {
	// Check, if waiting position and already at target
	if (waiting == WaitPos && _motor->currentPos == _motor->targetPos) {
		// Stop moving
		HAL_TIM_Base_Stop_IT(_motor->timHandler);
		// Report of end
		sendUSBreachPos();
		waiting = WaitNone;
		state = Waiting;
		return;
	}
	// Move
	HAL_GPIO_TogglePin(_motor->portPul, _motor->pinPul);
	// Changing position
	_motor->currentPos += _motor->direction;
}

int motorIsFinish(const Motor* _motor) {
	return _motor->currentPos == _motor->targetPos;
}

int motorGetPosition(const Motor* _motor) {
	return _motor->currentPos;
}

float motorGetLocation(const Motor* _motor) {
	return _motor->currentPos / _motor->gearboxRatio / _motor->substepsCount / 2.0;
}
