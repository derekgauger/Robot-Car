/**
 * @file LineSensor.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * This code is developed as part of the MSOE SE3910 Real Time Systems course,
 * but can be freely used by others.
 *
 * SE3910 Real Time Systems is a required course for students studying the
 * discipline of software engineering.
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * @section DESCRIPTION
 *      This file defines the interface for the class which is responsible for looking
 *      for lines.  In essence, if all 3 sensors are darkened, the robot will believe it is a stop request and stop the robot.
 */
#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "PeriodicTask.h"
#include "GPIO.h"
#include "CommandQueue.h"

namespace se3910RPi {

class LineSensor: public PeriodicTask {
private:
	/**
	 * This is the queue that this item will use to receive enable and disable commands from the network.
	 */
	CommandQueue *ctrlQueue;

	/**
	 * This is the queue that the line sensor interacts with for controlling the motor.  This queue will enque stop commands when they are active.
	 */
	CommandQueue *mcq;

	/**
	 * This boolean will determine whether or not this is currently active and should eb looking for lines that would cause the robot to stop.
	 */
	bool currentlyActive;

	/**
	 * This boolean will be true if the unit is to do steering.
	 */
	bool lineFollowingEnabled;

	/**
	 * This is the GPIO instance for the left sensor.
	 */
	GPIO *leftSensor;

	/**
	 * This is the GPIO instance for the center sensor.
	 */
	GPIO *centerSensor;

	/**
	 * This is the GPIO instance for the right sensor.
	 */
	GPIO *rightSensor;

public:
	/**
	 * This is the constructor for this class.
	 * @param ctrlQueue This is the queue that this item will use to receive enable and disable commands from the network.
	 * @param mcq This is the command queue used by the motor for control purposes.
	 * @param leftSensorNumber This is the GPIO pin number for the left sensor.
	 * @param centerSensorNumber This is the GPIO pin number for the center sensor.
	 * @param rightSensor This is the GPIO pin number for the right sensor.
	 * @param threadName This is the name of the thread that is to periodically be invoked.
	 * @param period This is the period for the task.  This period is given in microseconds.
	 */
	LineSensor(CommandQueue *ctrlQueue, CommandQueue* mcq, int leftSensorNumber, int centerSensorNumber, int rightSensorNumber, std::string threadName, uint32_t period);

	/**
	 * This is the default destructor which will clean up from the distance sensor.
	 */
	virtual ~LineSensor();

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time.
	 */
	virtual void taskMethod();
};

}

#endif /* LineSensor */
