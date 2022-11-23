/**
 * @file NetworkManager.cpp
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
 * This file defines the implementation for the Network Manager.  The Network Manager manages network connections and acts as a server, receiving messages sent over a socket.
 */

#include "NetworkManager.h"
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

/**
 * This is the constructor for the Network Manager.  It will instantiate a new instance of the class.
 * @param port This is the port that the network manager is to listen on for incoming connections.
 * @param queue This is the queue that is to be used for enqueueing received requests.
 * @param threadName This is the name given to the executing thread.  It is a simple, string that is human readable.
 */
NetworkManager::NetworkManager(unsigned short port, CommandQueue **queue,
		std::string threadName) :
		RunnableClass(threadName) {
	portNumber = port;
	myThread = NULL;
	keepGoing = true;
	referencequeue = queue;
}

/**
 * This is the destructor for the class which will clean up from the instantiation and operation of the class.
 */
NetworkManager::~NetworkManager() {
	/**
	 * Right now, this destructor doesn't have anything to do, as there are no dynamically allocated objects that need to be destroyed.
	 */
}

/**
 * This method will override the default stop method.  In doing so, it must call the base class's stop method prior to invoking it's own logic.
 */
void NetworkManager::stop() {
	/**
	 * 1.0 Call the parent's stop method.
	 */
	RunnableClass::stop();

	/**
	 * 2.0 Call shutdown on the socket to shut down the full-duplex connection.
	 */
	shutdown(server_fd, SHUT_RDWR);
}

/**
 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
 */
void NetworkManager::run() {
	int valread;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	int opt = 1;
	int addrlen = sizeof(clientAddress);
	int socketOpen;
	networkMessageStruct receivedMessage;

	/**
	 * 1.0 Create a socket file descriptor.  The socket is a tcp socket.
	 * If there is an error, indicate that the socket failed and return from the message.
	 */
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		return;
	}

	/**
	 * 2.0 Call setsockopt to setup the options on the socket.  Make sure the options are set at the sopcket level,
	 * allow the reuse of local addresses, as well as ports.  If there is an error, print out an error message and then
	 * return.
	 */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt))) {
		perror("setsockopt");
		return;
	}
	/**
	 * 3.0 The connection is to be using an internet based format.
	 */
	serverAddress.sin_family = AF_INET;

	/**
	 * 4.0 The connection can be made from any address.
	 */
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	/**
	 * 5.0 Setup which port to listen on.  The port must be converted to net endianess.
	 */
	serverAddress.sin_port = htons(portNumber);

	/**
	 * 6.0 Now bind the listening socket to the correct port.  If there is a failure, print out an error and exit the program.
	 */
	if (bind(server_fd, (struct sockaddr *) &serverAddress,
			sizeof(serverAddress)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/**
	 * 7.0 Loop so long as the thread is to continue running.
	 */
	while (keepGoing) {
		/**
		 * 7.1 Listen for and accept the given connection.  If there is an error, print out an error and exit.
		 */
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}

		/**
		 * 7.2 Accept the connection from the client, setting the connected socket to the value returned from the accept call.
		 * If there is an error and the thread is to keep going, return after printing out an error if the thread is to continue running.
		 */
		if (((connectedSocket = accept(server_fd, (struct sockaddr*) &clientAddress, (socklen_t*) &addrlen)) < 0)
				&& (keepGoing)) {
			perror("connect");
			return;
		}
		socketOpen = sizeof(networkMessageStruct);

		/**
		 * 7.3 So long as the socket remains open and the thread continues running.
		 */
		while ((socketOpen > 0) && (keepGoing)) {
			valread = 0;
			char* buf = (char*) &receivedMessage;

			/**
			 * 7.3.1 Receive the message.  Make sure to wait on all of the message being received.
			 */
			valread = recv(connectedSocket, buf, sizeof(networkMessageStruct), MSG_WAITALL);

			/**
			 * 7.3.2 Check to see if 0 bytes were received.
			 */
			if (valread == 0) {
				/**
				 * 7.3.2.1 If we receive 0 bytes, the socket has been closed so abort.
				 */
				socketOpen = 0;
				close(connectedSocket);
				connectedSocket = 0;
			} else {
				/**
				 * 7.3.3.1 Otherwise, Convert the message to the appropriate endian format.
				 * Do this by converting each individual structure element accordingly.
				 */
				receivedMessage.messageID = ntohl(receivedMessage.messageID);
				receivedMessage.timestampHigh = ntohl(receivedMessage.timestampHigh);
				receivedMessage.timestampLow = ntohl(receivedMessage.timestampLow);
				receivedMessage.messageType = ntohl(receivedMessage.messageType);
				receivedMessage.message = ntohl(receivedMessage.message);
				receivedMessage.messageDestination = ntohl(receivedMessage.messageDestination);
				receivedMessage.xorChecksum = ntohl(receivedMessage.xorChecksum);

				/**
				 * 7.3.3.2 Calculate the XOR checksum of the data received, not including the checksum field.
				 */
				int calculatedChecksum = receivedMessage.messageID
						^ receivedMessage.timestampHigh
						^ receivedMessage.timestampLow
						^ receivedMessage.messageType ^ receivedMessage.message
						^ receivedMessage.messageDestination;

				/**
				 * 7.3.3.3 Verify that the received checksum matchess the checksum that was transmitted.
				 **/
				if (calculatedChecksum == receivedMessage.xorChecksum) {
					/**
					 * 7.3.3.3.1 The message is valid.  Enqueue it to the right queue if the destination queue is valid and it is a COMMAND_MSG_TYPE.
					 */
					if ((receivedMessage.messageType == COMMAND_MSG_TYPE) &&
						(receivedMessage.messageDestination > 0) &&
						(receivedMessage.messageDestination <= NUMBER_OF_QUEUES)) {
						(*(referencequeue[receivedMessage.messageDestination - 1])).enqueue(receivedMessage.message);
					}
				}
			}
		}
	}
}

/**
 * This method will return the socket ID for the given class.
 * @return The socket ID will be returned.
 */
int NetworkManager::getSocketID() {
	return connectedSocket;
}

