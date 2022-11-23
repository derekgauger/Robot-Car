
/**
 * @file NetworkTransmissionManager.cpp
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
 * This file defines the implementation for the Network Transmission Manager.  The Network Transmission Manager returns status information back over the socket.
 */

#include "NetworkTransmissionManager.h"
#include "NetworkCfg.h"
#include "CommandQueue.h"
#include "NetworkMessage.h"
#include "NetworkCommands.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

using namespace std;

NetworkTransmissionManager::NetworkTransmissionManager(
		NetworkManager *associatedReceptionManager, std::string threadName) :
		RunnableClass(threadName) {
	this->associatedReceptionManager = associatedReceptionManager;
	/**
	 * Initialize the semaphore to be a counting sem with nothing on it.
	 **/
	sem_init(&queueCountSemaphore, 0, 0);

}

NetworkTransmissionManager::~NetworkTransmissionManager() {
	/**
	 * Nothing is needed here for now, as there is nothing dynamically created that we need to clean up afterwards.
	 */
}

/**
 * This method will override the default stop method.  In doing so, it must call the base class's stop method prior to invoking it's own logic.
 */
void NetworkTransmissionManager::stop() {

	/**
	 * Call the parent's stop method.
	 */
	RunnableClass::stop();

	networkMessageStruct endMsg;

	enqueueMessage(endMsg);

}

void NetworkTransmissionManager::enqueueMessage(networkMessageStruct &itemToEnqueue) {
	// Lock the queue.
	std::lock_guard<std::mutex> guard(queueMutex);
	// Place the given item on the end of the queue.
	transmissionQueue.push(itemToEnqueue);
	// Indicate that something has been enqueued through the semaphore.
	sem_post(&queueCountSemaphore);
}

/**
 * This is the virtual run method.  It will execute the given code that is to be executed by this class.
 */
void NetworkTransmissionManager::run() {
	while (keepGoing) {
		networkMessageStruct itemToTransmit;

		/**
		 * Block if there is nothing on the queue until something is enqueued.
		 */
		sem_wait(&queueCountSemaphore);
		{
			/**
			 * Lock the queue and dequeue an item from it.
			 */
			std::lock_guard<std::mutex> guard(queueMutex);

			/**
			 * Obtain the first item from the queue and then dispose of it.
			 */
			itemToTransmit = transmissionQueue.front();
			transmissionQueue.pop();
		}

		// Now that we have an item to transmit,
		itemToTransmit.message = htonl(itemToTransmit.message);
		itemToTransmit.messageDestination = htonl(itemToTransmit.messageDestination);
		itemToTransmit.xorChecksum = htonl(itemToTransmit.xorChecksum);

		if (associatedReceptionManager->getSocketID() > 0) {
			// Now send it.
			send(associatedReceptionManager->getSocketID(), &itemToTransmit, sizeof(networkMessageStruct), 0);
		} else {
			// DO nothing, as we are not connected through a socket right now.
		}
	}
}
