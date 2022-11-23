/**
 * @file NetworkManager.h
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
 * This file defines the interface for the Network Manager.  The Network Manager manages network connections and acts as a server, receiving messages sent over a socket to the class.
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_


#include "CommandQueue.h"
#include "RunnableClass.h"
#include "NetworkCfg.h"
#include "NetworkMessage.h"
#include <string>


class NetworkManager: public RunnableClass {
private:
	/**
	 * This is the port that will be used for receiving messages over the network.
	 */
	unsigned short portNumber;
	/**
	 * This is a pointer to the array of queues that will be used to communicate with other devices.
	 */
	CommandQueue** referencequeue;

	/**
	 * This integer is the file descriptor for the socket that is going to be listened to for connections.
	 */
	int server_fd=0;

	/**
	 * This is the connected socket of the network manager.  This will be updated whenever a connection is made and it is used to send messages back to the master as well as to receive messages from the master.
	 */
	int connectedSocket=0;

public:
	/**
	 * This is the constructor for the Network Manager.  It will instantiate a new instance of the class.
	 * @param port This is the port that the network manager is to listen on for incoming connections.
	 * @param queue This is the array of pointers to queues that is to be used for enqueueing received requests.
	 * @param threadName This is the name given to the executing thread.  It is a simple, string that is human readable.
	 */
	NetworkManager(unsigned short port, CommandQueue* queue[], std::string threadName);

	/**
	 * This is the destructor for the class which will clean up from the instantiation and operation of the class.
	 */
	virtual ~NetworkManager();
	/**
	 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
	 */
	void run();

	/**
	 * This method will override the default stop method.  In doing so, it must call the base class's stop method prior to invoking it's own logic.
	 */
	void stop();

	/**
	 * This method will return the socket ID for the given class.
	 * @return The socket ID will be returned.
	 */
	int getSocketID();
};



#endif /* NETWORKMANAGER_H_ */
