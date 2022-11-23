#include "DistanceSensor.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace se3910RPi;

namespace se3910RPiHCSR04 {

/**
 * This method will instantiate a new instance of the distance sensor.  The algorithm is as follows:
 * @param trigPin This is the trigger pin, which is the pin that determines when to start an ultrasonic measurement.
 * @param echoPin This is the echo pin.  It will indicate the distance to the remote device.
 * @param threadName This is the name of the thread that is to periodically be invoked.
 * @param period This is the period for the task.  This period is given in microseconds.
 */
DistanceSensor::DistanceSensor(int trigPin, int echoPin, std::string threadName,
		uint32_t period) :
		PeriodicTask(threadName, period) {

	// Setting the trigger pin
	tPin = new se3910RPi::GPIO(trigPin, GPIO::GPIO_OUT, GPIO::GPIO_LOW);

	// Setting the echo pin
	ePin = new se3910RPi::GPIO(echoPin, GPIO::GPIO_IN);

	// Enabling the edge interrupts both ways
	ePin->enableEdgeInterrupt(GPIO::GPIO_BOTH);

}

/**
 * This is the default destructor which will clean up from the distance sensor.
 * The method simply deletes allocated objects.
 */
DistanceSensor::~DistanceSensor() {

	delete ePin;
	delete tPin;

}

/**
 * This is the task method.  The task method will be invoked periodically every taskPeriod
 * units of time.
 */
void DistanceSensor::taskMethod() {
	struct timespec mydelay = { 0, 10000 };
	struct timespec startTime;
	struct timespec endTime;

	readCount++;

	tPin->setValue(se3910RPi::GPIO::GPIO_HIGH);
	nanosleep(&mydelay, &startTime);
	tPin->setValue(se3910RPi::GPIO::GPIO_LOW);

	int status1 = ePin->waitForEdge(10);

	if(status1 == 0){
		startTime = ePin->getRisingISRTimestamp();

	}

	int status2 = ePin->waitForEdge(11);

	if(status2 == 0){
		endTime = ePin->getFallingISRTimestamp();
	}

	if(status1 != 0 || status2 != 0 ){
		return;
	}

	float timePassed = (endTime.tv_nsec - startTime.tv_nsec);

	int distance = (343 * (timePassed/2))/1000000;

	if(distance < 0 || distance > 2000){
		return;
	}

	dsMutex.lock();

	currentDistance = distance;
	totalOfAllDistances += currentDistance;
	distanceRecordingCount++;

	if(distance < minDistance){
		minDistance = currentDistance;

	} else if(distance > maxDistance){
		maxDistance = currentDistance;
	}

	validReadCount++;

	dsMutex.unlock();

}

/**
 * This method will obtain the maximum read distance.
 * @return The max distance read will be returned in units of mm.
 */
int DistanceSensor::getMaxDistance() {
	return this->maxDistance;
}

/**
 * This method will obtain the minimum read distance.
 * @return The min distance read will be returned in units of mm.
 */
int DistanceSensor::getMinDistance() {
	return this->minDistance;
}

/**
 * This method will obtain the current read distance.
 * @return The last valid distance read will be returned in units of mm.
 */
int DistanceSensor::getCurrentDistance() {
	return this->currentDistance;
}

/**
 * This method will return the read cpount, or the number of times the periodic task is invoked..
 * @return The valid count will be returned.
 */
uint32_t DistanceSensor::getReadCount() {
	return readCount;
}

/**
 * This method will return the valid count or the number of times a valid read has been obtained..
 * @return The valid count will be returned.
 */
uint32_t DistanceSensor::getValidReadCount() {
	return validReadCount;
}

/**
 * This method will reset the max and min values to their default ranges, allowing new ranges to be captured.
 */
void DistanceSensor::resetDistanceRanges() {
	maxDistance = 0;
	minDistance = 2500;
	totalOfAllDistances = 0;
	distanceRecordingCount = 0;
	readCount = 0;
	validReadCount = 0;
}

/**
 * This method will obtain the average read distance.
 * @return The last average distance read will be returned.
 */
int DistanceSensor::getAverageDistance() {

	int retVal = 0;

	if(distanceRecordingCount > 0){

		retVal = totalOfAllDistances / distanceRecordingCount;
	}

	return retVal;
}

/**
 * This method will obtain the number of times that a distance has been measured since the count was reset.
 * @return the Number of samples captured will be returned.
 */
int DistanceSensor::getDistanceCount() {
	return this->distanceRecordingCount;
}
}
