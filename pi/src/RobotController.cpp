/*
 * RobotController.cpp
 *
 *  Created on: Oct 5, 2022
 *      Author: se3910
 */



#include "RobotController.h"
#include "NetworkCommands.h"
#include "RobotCfg.h"
#include "TaskRates.h"
#include "CommandQueue.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace se3910RPi;

#define HORN_PERIOD 600
#define HORN_LENGTH 300

RobotController::RobotController(CommandQueue* queue, std::string threadName) : RunnableClass(threadName) {
	this->referencequeue = queue;

	leftFrontMotor = new MotorController(PCADEVICE_ADDR, LFMFCHANNEL, LFMRCHANNEL, "LF", MOTOR_CTRL_TASK_PERIOD);
	leftRearMotor = new MotorController(PCADEVICE_ADDR, LRMFCHANNEL, LRMRCHANNEL, "LR", MOTOR_CTRL_TASK_PERIOD);
	rightFrontMotor = new MotorController(PCADEVICE_ADDR, RFMFCHANNEL, RFMRCHANNEL, "RF", MOTOR_CTRL_TASK_PERIOD);
	rightRearMotor = new MotorController(PCADEVICE_ADDR, RRMFCHANNEL, RRMRCHANNEL, "RR", MOTOR_CTRL_TASK_PERIOD);

	leftFrontMotor->setSpeed(500);
	leftRearMotor->setSpeed(500);
	rightFrontMotor->setSpeed(500);
	rightRearMotor->setSpeed(500);
}

RobotController::RobotController(CommandQueue* queue, CommandQueue* hornQueue, std::string threadName) : RunnableClass(threadName) {
	this->referencequeue = queue;
	this->hornQueue = hornQueue;

	leftFrontMotor = new MotorController(PCADEVICE_ADDR, LFMFCHANNEL, LFMRCHANNEL, "LF", MOTOR_CTRL_TASK_PERIOD);
	leftRearMotor = new MotorController(PCADEVICE_ADDR, LRMFCHANNEL, LRMRCHANNEL, "LR", MOTOR_CTRL_TASK_PERIOD);
	rightFrontMotor = new MotorController(PCADEVICE_ADDR, RFMFCHANNEL, RFMRCHANNEL, "RF", MOTOR_CTRL_TASK_PERIOD);
	rightRearMotor = new MotorController(PCADEVICE_ADDR, RRMFCHANNEL, RRMRCHANNEL, "RR", MOTOR_CTRL_TASK_PERIOD);

	leftFrontMotor->setSpeed(500);
	leftRearMotor->setSpeed(500);
	rightFrontMotor->setSpeed(500);
	rightRearMotor->setSpeed(500);

}

RobotController::~RobotController() {
	delete leftFrontMotor;
	delete leftRearMotor;
	delete rightFrontMotor;
	delete rightRearMotor;
}

void RobotController::run() {

	leftFrontMotor->start();
	leftRearMotor->start();
	rightFrontMotor->start();
	rightRearMotor->start();

	while (keepGoing) {
		if (referencequeue->hasItem()) {
			int taskNumber = referencequeue->dequeue();

			int commandArg = taskNumber & 0xFFF;
			int command = taskNumber - commandArg;

			if (command == STEERINGOFFSETBITMAP) {
				processSteeringControlCommand(commandArg);

			} else if (command == MOTORDIRECTIONBITMAP) {
				processMotionControlCommand(commandArg);

			} else if (command == SPEEDDIRECTIONBITMAP) {
				processSpeedControlCommand(commandArg);

			}
		}

	}

	leftFrontMotor->stop();
	leftRearMotor->stop();
	rightFrontMotor->stop();
	rightRearMotor->stop();

}

void RobotController::waitForShutdown() {
	RunnableClass::waitForShutdown();
}

void RobotController::stop() {
	RunnableClass::stop();

	leftFrontMotor->stop();
	leftRearMotor->stop();
	rightFrontMotor->stop();
	rightRearMotor->stop();

	referencequeue->enqueue(STOP);
}

int RobotController::processMotionControlCommand(int value) {

	int hornCommand = HORN_PULSE_COMMAND;
	hornCommand = hornCommand | (HORN_LENGTH << 12);
	hornCommand = hornCommand | HORN_PERIOD;

	int F = 1;
	int R = -1;
	int S = 0;

	if (value == FORWARD) {
		leftFrontMotor->setDirection(F);
		leftRearMotor->setDirection(F);
		rightFrontMotor->setDirection(F);
		rightRearMotor->setDirection(F);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	} else if (value == LEFT) {
		leftFrontMotor->setDirection(R);
		leftRearMotor->setDirection(R);
		rightFrontMotor->setDirection(F);
		rightRearMotor->setDirection(F);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	} else if (value == RIGHT) {
		leftFrontMotor->setDirection(F);
		leftRearMotor->setDirection(F);
		rightFrontMotor->setDirection(R);
		rightRearMotor->setDirection(R);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	} else if (value == BACKWARD) {
		leftFrontMotor->setDirection(R);
		leftRearMotor->setDirection(R);
		rightFrontMotor->setDirection(R);
		rightRearMotor->setDirection(R);

		hornQueue->enqueue(hornCommand);

	} else if (value == (FORWARD + LEFT)) {
		leftFrontMotor->setDirection(S);
		leftRearMotor->setDirection(S);
		rightFrontMotor->setDirection(F);
		rightRearMotor->setDirection(F);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	} else if (value == (FORWARD + RIGHT)) {
		leftFrontMotor->setDirection(F);
		leftRearMotor->setDirection(F);
		rightFrontMotor->setDirection(S);
		rightRearMotor->setDirection(S);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	} else if (value == (BACKWARD + LEFT)) {
		leftFrontMotor->setDirection(S);
		leftRearMotor->setDirection(S);
		rightFrontMotor->setDirection(R);
		rightRearMotor->setDirection(R);

		hornQueue->enqueue(hornCommand);

	} else if (value == (BACKWARD + RIGHT)) {
		leftFrontMotor->setDirection(R);
		leftRearMotor->setDirection(R);
		rightFrontMotor->setDirection(S);
		rightRearMotor->setDirection(S);

		hornQueue->enqueue(hornCommand);

	} else if (value == STOP) {
		leftFrontMotor->setDirection(S);
		leftRearMotor->setDirection(S);
		rightFrontMotor->setDirection(S);
		rightRearMotor->setDirection(S);

		hornQueue->enqueue(HORN_MUTE_COMMAND);

	}

	return value;

}

void RobotController::processSpeedControlCommand(int value) {
	currentSpeed = value;

	if (value > 1000 || value < 0) {
		return;
	}

	currentSpeed = value;

	if (currentSteering == 0) {
		leftFrontMotor->setSpeed(currentSpeed);
		leftRearMotor->setSpeed(currentSpeed);
		rightFrontMotor->setSpeed(currentSpeed);
		rightRearMotor->setSpeed(currentSpeed);

	} else if (currentSteering > 0) {
		double runPercent = (100.0 - (double) currentSteering) / 100.0;

		leftFrontMotor->setSpeed(currentSpeed);
		leftRearMotor->setSpeed(currentSpeed);

		rightFrontMotor->setSpeed(currentSpeed * runPercent);
		rightRearMotor->setSpeed(currentSpeed * runPercent);

	} else if (currentSteering < 0) {
		double runPercent = (100.0 - (double) -currentSteering) / 100.0;

		leftFrontMotor->setSpeed(currentSpeed * runPercent);
		leftRearMotor->setSpeed(currentSpeed * runPercent);

		rightFrontMotor->setSpeed(currentSpeed);
		rightRearMotor->setSpeed(currentSpeed);

	}

}

void RobotController::processSteeringControlCommand(int value) {

	value -= 100;

	if (value > 100 || value < -100) {
		return;
	}

	currentSteering = value;

	if (currentSteering == 0) {
		leftFrontMotor->setSpeed(currentSpeed);
		leftRearMotor->setSpeed(currentSpeed);
		rightFrontMotor->setSpeed(currentSpeed);
		rightRearMotor->setSpeed(currentSpeed);

	} else if (currentSteering > 0) {
		double runPercent = (100.0 - (double) currentSteering) / 100.0;

		leftFrontMotor->setSpeed(currentSpeed);
		leftRearMotor->setSpeed(currentSpeed);

		rightFrontMotor->setSpeed(currentSpeed * runPercent);
		rightRearMotor->setSpeed(currentSpeed * runPercent);

	} else if (currentSteering < 0) {
		double runPercent = (100 - (double) -currentSteering) / 100.0;

		leftFrontMotor->setSpeed(currentSpeed * runPercent);
		leftRearMotor->setSpeed(currentSpeed * runPercent);

		rightFrontMotor->setSpeed(currentSpeed);
		rightRearMotor->setSpeed(currentSpeed);

	}
}
