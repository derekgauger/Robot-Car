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

#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <pthread.h>
#include <string>

#include "CommandQueue.h"
#include "MotorController.h"
#include "RunnableClass.h"
#include "labcfg.h"


using namespace se3910RPi;

class RobotController : public RunnableClass {
protected:
	/**
	 * This is a pointer to the queue that will be used for receiving commands.  Commands can come from the network or other portions of the robot.
	 */
	CommandQueue* referencequeue;

	/**
	 * This is a pointer to the queue that will be sued to send commands to the horn.
	 */
	CommandQueue* hornQueue=NULL;


	/**
	 * This is a pointer to the left motor within the robot.  This will drive the left front wheel.
	 */
	MotorController* leftFrontMotor;

	/**
	 * This is a pointer to the left motor within the robot.  This will drive the left rear wheel.
	 */
	MotorController* leftRearMotor;

	/**
	 * This is a pointer to the right motor which drives the right front wheel.
	 */
	MotorController* rightFrontMotor;
	/**
	 * This is a pointer to the right motor which drives the right rear wheel.
	 */
	MotorController* rightRearMotor;

	/**
	 * This is the current speed for the robot.  It can vary between 0 and 1000.  It represents the basic PWM value for the robot to the nearest .1%.
	 */
	int currentSpeed=0;

	/**
	 * This is a value for the current steering control.  This value can range between -100 and +100, with 0 being straight ahead.
	 * It is used to adjust the speed of the motors relative to each other on the left and right side of the robot.
	 * A value of -100 will turn the robot to the left, causing the left wheels to be completely stopped;
	 * a value of 100 will turn the robot to the right, causing the right wheels to be completely stopped.
	 */
	int currentSteering = 0;

	/**
	 * This is the current operation that the robot is performing, ie forward, left, right, etc.
	 */
	int currentOperation=0;

	/**
	 * This method will process a command that is related to motion control.
	 * @param value This is the command that was received.
     * @return This is the command that was last processed.  Under normal circumstances, it is exactly the value as the value passed in.
	 */
	int processMotionControlCommand(int value);

	/**
	 * This method will process a command that is related to speed control.
	 * It will first verify that the value passed in is in range and then will update the motors accordingly based on the new
	 * parameter passed in.
	 * @param value This is the command that was received.  It is expected to be a value between 0 and 1000 (inclusive).
	 */
	void processSpeedControlCommand(int value);

	/**
	 * This method will process a command that is related to steering control.
	 * It will first verify that the value passed in is in range and then will update the motors accordingly based on the new
	 * parameter passed in.
	 * @param value This is the command that was received.  It is expected to be a value between -100 and 100 (inclusive).
	 */
	void processSteeringControlCommand(int value);

public:

	/**
	 * This method will instantiate a new instance of the robot controller.
	 * @param queue This is the instance of the queue that will be used to send commands to the robot.
	 * @param threadName This is the name given to the executing thread of this class.  It is a simple, string that is human readable.
	 */
	RobotController(CommandQueue* queue, std::string threadName);

#if LAB_IMPLEMENATION_STEP < 1
	// DO nothing.
#else
	/**
	 * This method will instantiate a new instance of the robot controller.
	 * @param queue This is the instance of the queue that will be used to send commands to the robot.
	 * @param hornQueue This is the queue that will be used for the horn.
	 * @param threadName This is the name given to the executing thread of this class.  It is a simple, string that is human readable.
	 */
	RobotController(CommandQueue* queue, CommandQueue* hornQueue, std::string threadName);

#endif


	/**
	 * This method destroys the existing instance of the robot controller.
	 */
	virtual ~RobotController();

	/**
	 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
	 */
	void run();

	/**
	 * this method will stop the thread and its execution, as well as the robot.
	 */
	void stop();

	/**
	 * This method will block waiting for the given thread to terminate before continuing.  This should be overridden if any child class
	 * has it's own runnable objects encapsulated within it.
	 *
	 */
	void waitForShutdown();
};

#endif /* ROBOTCONTROLLER_H */
