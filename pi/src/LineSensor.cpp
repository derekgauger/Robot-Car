/*
 * LineSensor.cpp
 *
 *  Created on: Oct 26, 2022
 *      Author: se3910
 */


#include "PeriodicTask.h"
#include "GPIO.h"
#include "CommandQueue.h"
#include "LineSensor.h"
#include "NetworkCommands.h"
#include <iostream>

using namespace std;
using namespace se3910RPi;

LineSensor::LineSensor(CommandQueue* ctrlQueue, CommandQueue* mcq,
		int leftSensorNumber, int centerSensorNumber, int rightSensorNumber,
		string threadName, uint32_t period) : PeriodicTask(threadName, period) {
	this->ctrlQueue = ctrlQueue;
	this->mcq = mcq;

	leftSensor = new GPIO(leftSensorNumber, GPIO::GPIO_IN);
	centerSensor = new GPIO(centerSensorNumber, GPIO::GPIO_IN);
	rightSensor = new GPIO(rightSensorNumber, GPIO::GPIO_IN);

	lineFollowingEnabled = false;
	currentlyActive = false;
}

LineSensor::~LineSensor() {
	delete leftSensor;
	delete centerSensor;
	delete rightSensor;
}

void LineSensor::taskMethod() {
	if (ctrlQueue->hasItem()) {
		int event = ctrlQueue->dequeue();
		if (event == STOP_LINE_SENSING) {
			currentlyActive = false;
		} else if (event == START_LINE_SENSING) {
			currentlyActive = true;
		} else if (event == DISABLE_LINE_FOLLOWING) {
			lineFollowingEnabled = false;
		} else if (event == ENABLE_LINE_FOLLOWING) {
			lineFollowingEnabled = true;
		}
	}

	if (currentlyActive) {
		int leftRead = leftSensor->getValue();
		int centerRead = centerSensor->getValue();
		int rightRead = rightSensor->getValue();

		if (leftRead == centerRead && centerRead == rightRead && rightRead == GPIO::GPIO_HIGH) {
			mcq->enqueue(MOTORDIRECTIONBITMAP | STOP);

		} else {
			
			if (lineFollowingEnabled) {

				if (leftRead == GPIO::GPIO_LOW && centerRead == GPIO::GPIO_HIGH && rightRead == GPIO::GPIO_LOW) {
					mcq->enqueue(MOTORDIRECTIONBITMAP | FORWARD);

				} else if (leftRead == GPIO::GPIO_HIGH && rightRead == GPIO::GPIO_LOW) {
					mcq->enqueue(MOTORDIRECTIONBITMAP | LEFT);

				} else if (leftRead == GPIO::GPIO_LOW && rightRead == GPIO::GPIO_HIGH) {
					mcq->enqueue(MOTORDIRECTIONBITMAP | RIGHT);

				} else if (leftRead == GPIO::GPIO_HIGH && rightRead == GPIO::GPIO_HIGH) {
					mcq->enqueue(MOTORDIRECTIONBITMAP | STOP);
				}
			}
		}

	}
}
