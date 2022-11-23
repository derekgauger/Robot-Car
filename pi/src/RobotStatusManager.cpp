/*
 * RobotStatusManager.cpp
 *
 *  Created on: Nov 1, 2020
 *      Author: se3910
 */

#include "RobotStatusManager.h"
#include "NetworkMessage.h"
#include "NetworkCommands.h"

namespace se3910RPi {

	RobotStatusManager::RobotStatusManager(NetworkTransmissionManager *nti, se3910RPiHCSR04::DistanceSensor* dsi, std::string threadName, uint32_t period) : PeriodicTask(threadName, period)
	{
		this->nti = nti;
		this->dsi = dsi;
	}

	RobotStatusManager::~RobotStatusManager()
	{

	}

	void RobotStatusManager::taskMethod()
	{
		/**
		 * 1.0 Start by reading the current, max, min, and average distance from the distance sensor.  Then reset the sensor.
		 */
		int currentDistance = dsi->getCurrentDistance();
		int maxDistance = dsi->getMaxDistance();
		int minDistance = dsi->getMinDistance();
		int aveDistance = dsi->getAverageDistance();
		dsi->resetDistanceRanges();

		/**
		 * 2.0 Now populate an instance of the network message structure for current distance.  The destination device is 1.  The message is the current distance ored with the appropriate message parameter.
		 */
		networkMessageStruct nms;
		nms.messageDestination=1;
		nms.message = DISTANCE_MEASUREMENT_REPORT | DISTANCE_MEASUREMENT_REPORT_CURRENTREADINGBITMAP | currentDistance;
		nms.xorChecksum = nms.message ^ nms.messageDestination;

		/**
		 * 3.0 Enqueue the item to be sent.
		 */
		nti->enqueueMessage(nms);


		/**
		 * 4.0 Now populate an instance of the network message structure for max distance.  The destination device is 1.  The message is the current distance ored with the appropriate message parameter.
		 */
		nms.messageDestination=1;
		nms.message = DISTANCE_MEASUREMENT_REPORT | DISTANCE_MEASUREMENT_REPORT_MAXREADINGBITMAP | maxDistance;
		nms.xorChecksum = nms.message ^ nms.messageDestination;

		/**
		 * 5.0 Enqueue the item to be sent.
		 */
		nti->enqueueMessage(nms);

		/**
		 * 6.0 Now populate an instance of the network message structure for max distance.  The destination device is 1.  The message is the current distance ored with the appropriate message parameter.
		 */
		nms.messageDestination=1;
		nms.message = DISTANCE_MEASUREMENT_REPORT | DISTANCE_MEASUREMENT_REPORT_MINREADINGBITMAP | minDistance;
		nms.xorChecksum = nms.message ^ nms.messageDestination;

		/**
		 * 7.0 Enqueue the item to be sent.
		 */
		nti->enqueueMessage(nms);

		/**
		 * 8.0 Now populate an instance of the network message structure for max distance.  The destination device is 1.  The message is the current distance ored with the appropriate message parameter.
		 */
		nms.messageDestination=1;
		nms.message = DISTANCE_MEASUREMENT_REPORT | DISTANCE_MEASUREMENT_REPORT_AVEREADINGBITMAP | aveDistance;
		nms.xorChecksum = nms.message ^ nms.messageDestination;

		/**
		 * 9.0 Enqueue the item to be sent.
		 */
		nti->enqueueMessage(nms);
	}



}
