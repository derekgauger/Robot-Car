/**
 * @file RobotStatusManager.h
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
 *      This file defines the class which is responsible for returning status from the robot.  The status that is returned can vary based upon what the user desires to report.
 */
#ifndef ROBOTSTATUSMANAGER_H
#define ROBOTSTATUSMANAGER_H

#include "PeriodicTask.h"
#include "CommandQueue.h"
#include "DistanceSensor.h"
#include "NetworkTransmissionManager.h"

namespace se3910RPi {

class RobotStatusManager: public PeriodicTask {
private:
	/**
	 * This is the instance of the distance sensor that is to be used in this project.
	 */
	se3910RPiHCSR04::DistanceSensor* dsi;

	/**
	 * This is the instance of the network transmitter that is to be sued to transmit information back to the base station / Java Program.
	 */
	NetworkTransmissionManager *nti;

public:
	RobotStatusManager(NetworkTransmissionManager *nti, se3910RPiHCSR04::DistanceSensor* dsi, std::string threadName, uint32_t period);

	/**
	 * This is the default destructor which will clean up from the distance sensor.
	 */
	virtual ~RobotStatusManager();

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time.
	 */
	virtual void taskMethod();
};

}

#endif /* LineSensor */
