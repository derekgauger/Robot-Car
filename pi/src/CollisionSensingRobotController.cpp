/*
 * CollisionSensingRobotController.cpp
 *
 *  Created on: Nov 2, 2022
 *      Author: se3910
 */

#include "RobotController.h"
#include "CollisionSensor.h"
#include "CollisionSensingRobotController.h"

using namespace std;
using namespace se3910RPi;

/**
 * This method will instantiate a new instance of the robot controller.
 * @param queue This is the instance of the queue that will be used to send commands to the robot.
 * @param threadName This is the name given to the executing thread of this class.  It is a simple, string that is human readable.
 */
CollisionSensingRobotController::CollisionSensingRobotController(CommandQueue* queue,
		CommandQueue* hornQueue, CollisionSensor* cs,
		string threadName) : RobotController(queue, hornQueue, threadName){
	this->cs = cs;
}

/**
 * This method will process a command that is related to speed control.
 * It will first verify that the value passed in is in range and then will update the motors accordingly based on the new
 * parameter passed in.  It will then update the stopping distance for the collision sensor.
 * @param value This is the command that was received.  It is expected to be a value between 0 and 1000 (inclusive).
 */
void CollisionSensingRobotController::processSpeedControlCommand(int value) {
	if (value >= 0 && value <= 1000) {
		processSpeedControlCommand(value);
		// TODO find mad
		// cs->setMinimumAcceptableDistance(mad);
	}
}

/**
 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
 */
void CollisionSensingRobotController::run() {
	RunnableClass::start();
}
