/**
 * @file NetworkMessage.h
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
 *
 */

#ifndef NETWORKTRANSMISSIONMANAGER_H
#define NETWORKTRANSMISSIONMANAGER_H

#include "RunnableClass.h"
#include "NetworkCfg.h"
#include "NetworkMessage.h"
#include <string>
#include "NetworkManager.h"


class NetworkTransmissionManager: public RunnableClass {
private:
	NetworkManager *associatedReceptionManager;
	std::queue<networkMessageStruct> transmissionQueue;
	/**
	 * This is a counting semaphore which keeps track of how many items are on the queue.
	 */
	sem_t queueCountSemaphore;
	/**
	 * This is a mutex for the class which is used to lock critical sections in different methods.
	 */
	std::mutex queueMutex;


public:
	NetworkTransmissionManager(NetworkManager* associatedReceptionManager, std::string threadName);
	virtual ~NetworkTransmissionManager();
	void enqueueMessage(networkMessageStruct &itemToEnqueue);

	/**
	 * This method will override the default stop method.  In doing so, it must call the base class's stop method prior to invoking it's own logic.
	 */
	void stop();

	/**
	 * This is the virtual run method.  It will execute the given code that is to be executed by this class.
	 */
	virtual void run();

};

#endif
