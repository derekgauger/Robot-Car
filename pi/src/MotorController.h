/**
 * @file MotorController.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
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
 * This header represents an interface for a PWM controlled Motor.
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_


#include "PeriodicTask.h"
#include "PCA9685Driver.h"
#include <string>

class MotorController : public PeriodicTask {
private:
	/**
	 * This is a pointer to the instance of the PCA control hardware.
	 */
	PCA9685Driver *controlHardware;

	/**
	 * This variable holds the channel that will control the given motor.  The fchannel is the channel for PWM if the motor is going forward.
	 */
	uint16_t fchannel;

	/**
	 * This variable holds the channel that will control the given motor.   The rchannel is the channel for reverse.
	 */
	uint16_t rchannel;

	/**
	 * This is the speed for the motor.  It can range between 0 and 100%.  100% is a value of 1000 to allow for steps of .1% from the integer value.
	 */
	int speed = 0;

	/**
	 * This is the direction for the motor.  0 is stopped.  1 is forward.  -1 is reverse.
	 */
	int direction = 0;

public:
	/**
	 * This is the overridden taskMethod that is periodically invoked.
	 */
	virtual void taskMethod();

	/**
	 * This constructor will instantiate a new instance of the Motor controller.
	 * @param deviceID This is the device ID for the motor.  Values are found in robotcfg.h
	 * @param fchannel This is the forward channel for the motor.  This is found in robotcfg.h
	 * @param rchannel This is the reverse channel for the motor.  This is defined in robotcfg.h
	 * @param threadName This is the name for the motor control thread.  The name should be unique to the thread so that you can easily tell which thread you are referring to.  Ideally, no two threads share a name in the system.
	 * @param period This si the period for the task.  These are defined in taskrates.h
	 */
	MotorController(uint16_t deviceID, uint16_t fchannel, uint16_t rchannel, std::string threadName, uint32_t period);

	/**
	 * This method will clean up from the PWM Motor controller.
	 */
	virtual ~MotorController();

	/**
	 * This method will set speed for the given motor.
	 * @param speed This is the speed.  Values outside of the range 0 to 1000 will be ignored.  0 represents 0.0% duty cycle, while 1000 represents 100.0% duty cycle.
	 */
	void setSpeed(int speed);

	/**
	 * This method will set the direction for the motor.
	 * @param dir The directions are as follows: negative indicates reverse, 0 indicates stopped, and positive indicates forward.
	 */
	void setDirection(int dir);

	/**
	 * this method will stop the motor, preventing it from spinning.
	 */
	void stop();
};

#endif /* MOTORCONTROLLER_H_ */
