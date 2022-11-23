/*
 * CollisionSensor.cpp
 *
 *  Created on: Oct 26, 2022
 *      Author: se3910
 */

#include "PeriodicTask.h"
#include "GPIO.h"
#include "DistanceSensor.h"
#include "CommandQueue.h"
#include "CollisionSensor.h"
#include "NetworkCommands.h"

using namespace std;
using namespace se3910RPi;

/**
 * This is the constructor for this class.
 * @param mcq This is the command queue used by the motor for control purposes.
 * @param hq This is the queue used to communicate with the Horn controller.
 * @param dsi This is the instance of the distance sensor.  It will read how far it is to a given object.
 * @param threadName This is the name of the thread that is to periodically be invoked.
 * @param period This is the period for the task.  This period is given in microseconds.
 */
CollisionSensor::CollisionSensor(CommandQueue* mcq, CommandQueue* hq,
		se3910RPiHCSR04::DistanceSensor* dsi, string threadName,
		uint32_t period) :
		PeriodicTask(threadName, period) {
	this->mcq = mcq;
	this->hq = hq;
	this->ds = dsi;
}

/**
 * This is the default destructor which will clean up from the distance sensor.
 */
CollisionSensor::~CollisionSensor() {

}

/**
 * This method will set the minimum distance.  If two reads in a row are below this value, then a stop command will be sent.
 * The stop command, however, is to be sent exactly once.
 * @param mad This is the minimum distance to the wall or other item that is acceptable before stopping.  This must be a positive number of no change will occur.
 */
void CollisionSensor::setMinimumAcceptableDistance(int mad) {
	this->minimumAcceptableDistance = mad;
}

/**
 * This is the task method.  The task method will be invoked periodically every taskPeriod
 * units of time.
 */
void CollisionSensor::taskMethod() {
	int currentDistance = ds->getCurrentDistance();
	if(currentDistance >= minimumAcceptableDistance) {
		madViolationCount = 0;
	} else {
		madViolationCount++;
		if(madViolationCount == 2) {
			mcq->enqueue(MOTORDIRECTIONBITMAP | STOP); // stop the engines
			hq->enqueue(HORN_SOUND_COMMAND); // sound the horn
		}
		if(madViolationCount == 3) {
			hq->enqueue(HORN_SOUND_COMMAND); // sound the horn
		}
	}

}
