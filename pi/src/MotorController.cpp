/*
 * MotorController.cpp
 *
 *  Created on: Oct 5, 2022
 *      Author: se3910
 */


#include "MotorController.h"
#include "PCA9685Driver.h"
#include <stdint.h>
#include <iostream>

using namespace std;

MotorController::MotorController(uint16_t deviceID, uint16_t fchannel, uint16_t rchannel, std::string threadName, uint32_t period) : PeriodicTask(threadName, period) {
	controlHardware = PCA9685Driver::obtainPCA9685Instance(deviceID);
	this->fchannel = fchannel;
	this->rchannel = rchannel;
}

MotorController::~MotorController() {

}

void MotorController::setSpeed(int speed) {
	if (speed <= 1000 || speed >= 0) {
		this->speed = speed;
	}
}

void MotorController::setDirection(int dir) {
	if (dir == -1 || dir == 0 || dir == 1) {
		this->direction = dir;
	}
}

void MotorController::taskMethod() {
	if (direction == 0) {
		controlHardware->setDutyCycle(rchannel, 0);
		controlHardware->setDutyCycle(fchannel, 0);

	} else if (direction == -1) {
		controlHardware->setDutyCycle(rchannel, speed);
		controlHardware->setDutyCycle(fchannel, 0);

	} else if (direction == 1) {
		controlHardware->setDutyCycle(rchannel, 0);
		controlHardware->setDutyCycle(fchannel, speed);

	}
}

void MotorController::stop() {
	PeriodicTask::stop();
	this->direction = 0;
}
