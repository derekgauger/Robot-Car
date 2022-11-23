/**
 * @file CollisionSensor.h
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
 *      for collisions and stopping the robot before a forward collision occurs.
 */
#ifndef COLLISIONSENSOR_H
#define COLLISIONSENSOR_H

#include "PeriodicTask.h"
#include "GPIO.h"
#include "DistanceSensor.h"
#include "CommandQueue.h"

namespace se3910RPi {

class CollisionSensor: public PeriodicTask {
private:
	/**
	 * This is the queue that the distance sensor interacts with to control the motor.  In essence, whenever this task runs, it determines the distance to an obstruction and if it is less than the given value two times running, it will stop the robot.
	 */
	CommandQueue *mcq;

	/**
	 * This is the queue that will be used to enque events for the horn controller to process.
	 */
	CommandQueue *hq;

	/**
	 * This is the instance of the distance sensor that is within this class.
	 * It will be passed into the constructor.
	 */
	se3910RPiHCSR04::DistanceSensor *ds;

	/**
	 * This is the minimum distance to the object that must be recorded twice before the robot will stop.
	 * The item is given in mm.
	 */
	int minimumAcceptableDistance=100;

	/**
	 * This variable keeps track of the number of times a reading less than the minimumacceptabledistance has been recorded.  If 2, it will send a stop command.  It will continue counting upwards until a value less that the mad occurs at which point it will reset to 0.
	 */
	int madViolationCount=0;

public:
	/**
	 * This is the constructor for this class.
	 * @param mcq This is the command queue used by the motor for control purposes.
	 * @param hq This is the queue used to communicate with the Horn controller.
	 * @param dsi This is the instance of the distance sensor.  It will read how far it is to a given object.
	 * @param threadName This is the name of the thread that is to periodically be invoked.
	 * @param period This is the period for the task.  This period is given in microseconds.
	 */
	CollisionSensor(CommandQueue* mcq, CommandQueue *hq, se3910RPiHCSR04::DistanceSensor* dsi, std::string threadName, uint32_t period);

	/**
	 * This is the default destructor which will clean up from the distance sensor.
	 */
	virtual ~CollisionSensor();

	/**
	 * This method will set the minimum distance.  If two reads in a row are below this value, then a stop command will be sent.
	 * The stop command, however, is to be sent exactly once.
	 * @param mad This is the minimum distance to the wall or other item that is acceptable before stopping.  This must be a positive number of no change will occur.
	 */
	void setMinimumAcceptableDistance(int mad);

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time.
	 */
	virtual void taskMethod();
};

}

#endif /* DISTANCESENSOR_H_ */
