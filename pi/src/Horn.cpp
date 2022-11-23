/*
 * Horn.cpp
 *
 *  Created on: Oct 26, 2022
 *      Author: se3910
 */


#include "GPIO.h"
#include "PeriodicTask.h"
#include "CommandQueue.h"
#include "Horn.h"
#include "TaskRates.h"
#include "NetworkCommands.h"
#include <iostream>

using namespace std;
using namespace se3910RPi;

Horn::Horn(CommandQueue *queue, int gpioPin, string threadName,
		uint32_t taskRate) : PeriodicTask(threadName, taskRate) {
	hornPin = new GPIO(gpioPin, GPIO::GPIO_OUT);
	hornPin->setValue(GPIO::GPIO_LOW); // TODO: supposed to be silent
	myqueue = queue;
}

Horn::~Horn() {
	delete hornPin;
}

void Horn::soundHorn() {
	// set the horn count to 0
	hornCount = 0;
	// set the repetition to 1
	repetitionTime = 1;
}

void Horn::silenceHorn() {
	// set the horn count to -1
	hornCount = -1;
}

void Horn::pulseHorn(int length, int period) {
	// set horn count to 0
	hornCount = 0;
	// set the length of the horn to be what was passed in
	this->length = length;
	// set the repetition time to be the period that was passed in
	repetitionTime = period;
}

void Horn::taskMethod() {
	if (myqueue->hasItem()) {
		int event = myqueue->dequeue();
		if (event == HORN_MUTE_COMMAND) {
			// TODO figure out which is silent or pulse horn
			silenceHorn();
		} else if (event == HORN_SOUND_COMMAND) {
			// TODO fix if needed, like above
			soundHorn();
		} else if ((event & 0xFF000000) == HORN_PULSE_COMMAND) {
			// TODO fix for pulseHorn event
			cout << ":::::::: " << event << endl;
			int period = (event & 0x00000FFF);
			length = (event & 0x00FFF000) >> 12;

			pulseHorn(length * 1000, period * 1000);
		}
	}

	if (hornCount == -1) {
		hornPin->setValue(GPIO::GPIO_LOW);
	} else if (hornCount == 0) {
		hornPin->setValue(GPIO::GPIO_HIGH);
		hornCount += HORN_TASK_PERIOD;
		hornCount = hornCount % repetitionTime;

	} else {
		if (hornCount <= length) {
			hornPin->setValue(GPIO::GPIO_HIGH);
		} else {
			hornPin->setValue(GPIO::GPIO_LOW);
		}
		hornCount += HORN_TASK_PERIOD;
		hornCount = hornCount % repetitionTime;

	}
}
