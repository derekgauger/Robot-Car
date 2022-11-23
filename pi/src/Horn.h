/**
 * @file Horn.h
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
 *      This interface defines the horn class.  The horn class makes audio noise through the piezo buzzer when desired.
 */

#ifndef HORN_H_
#define HORN_H_

#include "GPIO.h"
#include <thread>
#include "PeriodicTask.h"
#include "CommandQueue.h"

class Horn: public PeriodicTask {
public:
	/**
	 * This is the default constructor.
	 * @para, queue This is the queue that this task is to read from.
	 * @param gpioPin This is the GPIO pin that the buzzer is attached to.
	 * @param threadName This is the name that is to be given to the executing thread as a string.
	 * @param taskRate This is the rate at which the thread re-executes.
	 */
	Horn(CommandQueue *queue, int gpioPin, std::string threadName, uint32_t taskRate);
	/**
	 * This is the default destructor that will clean up from the class.
	 */
	virtual ~Horn();

	/**
	 * This method will cause the horn to sound continuously until stopped.
	 */
	void soundHorn();
	/**
	 * This method will pulse the horn.  It will be on for length ms and then off for the remainder of time until the period is reached.
	 * @param length This is the length of the on time, given in ms.
	 * @param period This is the period for the horn, given in ms.
	 */
	void pulseHorn(int length, int period);

	/**
	 * This method will silence the horn so that it no longer is making any audio noise.
	 */
	void silenceHorn();

	/**
	 * This is the task method for the class.
	 */
	void taskMethod();

private:

	/**
	 * This is the queue that commands are received on for controlling the horn.
	 */
	CommandQueue *myqueue;
	/**
	 * This variable is the physical device that the horn is connected to.
	 */
	se3910RPi::GPIO *hornPin;

	/**
	 * This is the current count, namely how many ms into the current period we are.
	 */
	int hornCount = -1;

	/**
	 * This is the length that the horn is to be on in ms.
	 */
	int length = 0;

	/**
	 * This is the length of time until repetition is to occur again in ms.
	 */
	int repetitionTime = 1;
};

#endif /* HORN_H_ */
