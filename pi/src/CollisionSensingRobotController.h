/**
 * @file RobotController.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.1
 *
 * @section LICENSE
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
 *      This class is a controller for the robot.  It is responsible for managing the operation of the motors based upon incoming commands
 *      from the command queue.
 */

#ifndef COLLISIONSENSINGROBOTCONTROLLER_H
#define COLLISIONSENSINGROBOTCONTROLLER_H

#include "RobotController.h"
#include "CollisionSensor.h"



class CollisionSensingRobotController : public RobotController {
private:
	se3910RPi::CollisionSensor* cs;

public:
	/**
	 * This method will instantiate a new instance of the robot controller.
	 * @param queue This is the instance of the queue that will be used to send commands to the robot.
	 * @param threadName This is the name given to the executing thread of this class.  It is a simple, string that is human readable.
	 */
	CollisionSensingRobotController(CommandQueue* queue, CommandQueue* hornQueue, se3910RPi::CollisionSensor* cs, std::string threadName);

	/**
	 * This method will process a command that is related to speed control.
	 * It will first verify that the value passed in is in range and then will update the motors accordingly based on the new
	 * parameter passed in.  It will then update the stopping distance for the collision sensor.
	 * @param value This is the command that was received.  It is expected to be a value between 0 and 1000 (inclusive).
	 */
	virtual void processSpeedControlCommand(int value);

	/**
	 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
	 */
	void run();
};

#endif

