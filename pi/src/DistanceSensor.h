/**
 * @file DistanceSensor.h
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
 *      This file defines the interface to be used to communicate with a
 *      HC-SR04 Ultrasonic distance sensor.  The sensor will give the
 * line of site distance to a remote object via ultrasonic detection.
 */
#ifndef DISTANCESENSOR_H_
#define DISTANCESENSOR_H_

#include <string.h>
#include <stdint.h>
#include <cfloat>
#include "PeriodicTask.h"
#include "GPIO.h"


namespace se3910RPiHCSR04 {

class DistanceSensor: public PeriodicTask {
private:
	/*
	 * This is a mutex for the class which will prevent race conditions within the code.
	 */
	std::mutex dsMutex;

	/**
	 * This is the GPIO pin for the trigger pin.  The trigger pin starts ultrasonic distance measuring.
	 */
	se3910RPi::GPIO *tPin;
	/**
	 * This is the GPIO pin for the echo pin.  Measuring this width gives the distance.
	 */
	se3910RPi::GPIO *ePin;

	/**
	 * This is the number of reads which have been attempted.  It will be incremented each time the period task attempts to read the value.
	 * This will be reset to 0 when the resetDistanceRange method is invoked.
	 */
	uint32_t readCount=0;

	/**
	 * The is the number of valid reads.  A valid read occurs if the distance sensor returns a value that is valid during testing.
	 * This will be reset to 0 when the resetDistanceRange method is invoked.
	 */
	uint32_t validReadCount=0;

	/**
	 * This is the maximum distance that has been recorded.  Units are stored as mm.
	 */
	int maxDistance = 0;

	/**
	 * This is the minimum distance that has been recorded.  This is stored in mm.
	 */
	int minDistance = 2500;

	/**
	 * This variable will keep track of how many distance reads have been completed.  It is used to calculate the average of the reads since the last reset.
	 */
	int distanceRecordingCount=0;

	/**
	 * This variable will keep track of the total value of all reads.  It will be used to keep track of the average reading.
	 */
	int totalOfAllDistances=0;

	/**
	 * This is the current distance from the last valid read.
	 */
	int currentDistance = 2500;

public:
	/**
	 * This method will instantiate a new instance of the distance sensor.
	 * @param trigPin This is the trigger pin, which is the pin that determines when to start an ultrasonic measurement.
	 * @param echoPin This is the echo pin.  It will indicate the distance to the remote device.
	 * @param threadName This is the name of the thread that is to periodically be invoked.
	 * @param period This is the period for the task.  This period is given in microseconds.
	 */
	DistanceSensor(int trigPin, int echoPin, std::string threadName,
			uint32_t period);

	/**
	 * This is the default destructor which will clean up from the distance sensor.
	 */
	virtual ~DistanceSensor();

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time.
	 */
	virtual void taskMethod();

	/**
	 * This method will obtain the maximum read distance.
	 * @return The max distance read will be returned in mm.
	 */
	virtual int getMaxDistance();

	/**
	 * This method will obtain the minimum read distance.
	 * @return The min distance read will be returned inmm.
	 */
	virtual int getMinDistance();

	/**
	 * This method will obtain the current read distance.
	 * @return The last valid distance read will be returned in mm.
	 */
	virtual int getCurrentDistance();

	/**
	 * This method will obtain the average read distance.
	 * @return The last average distance read will be returned in mm.
	 */
	virtual int getAverageDistance();

	/**
	 * This method will return the read cpount, or the number of times the periodic task is invoked..
	 * @return The valid count will be returned.
	 */
	virtual uint32_t getReadCount();

	/**
	 * This method will return the valid count or the number of times a valid read has been obtained..
	 * @return The valid count will be returned.
	 */
	virtual uint32_t getValidReadCount();

	/**
	 * This method will reset the max and min values to their default ranges, allowing new ranges to be captured.
	 */
	virtual void resetDistanceRanges();

	/**
	 * This method will obtain the number of times that a distance has been measured since the count was reset.
	 * @return the Number of samples captured will be returned.
	 */
	virtual int getDistanceCount();
};

} /* namespace se3910RPiHCSR04 */

#endif /* DISTANCESENSOR_H_ */
