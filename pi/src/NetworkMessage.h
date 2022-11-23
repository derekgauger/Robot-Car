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
 * This file defines a network message.  A network message is sent over the socket to control the robot.  Within this network message there are three parts.
 * The first 32 bit integer defines the destination for the message.  The second 32 bits defines the specific message.  The third 32 bits is a checksum which verifies that the message
 * is correct.
 */

#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H
#include  <cstdint>

#define COMMAND_MSG_TYPE (0x09)

/**
 * This structure represents a network message.
 */
struct networkMessageStruct {
	/**
	 * This is a unique ID for this message.  It should always increase by 1 each time a message is sent.  There never should be a duplicate message ID until the system rolls over after 2^32-1 messages, which should take a while.
	 */
	int32_t messageID;

	/**
	 * This is the timestamp for this message.  It is the number of ms since the start of the Epoch.
	 */
	int32_t timestampHigh;
	int32_t timestampLow;

	/**
	 * This is the type of message that is being transmitted.  COMMAND_MSG_TYPES are the only supported message types right now.  Anything else should be ignored.
	 */
	int32_t messageType;

	/**
	 * This is the destination.  This is a numerical destination on the given device for the message (i.e. the queue)
	 */
	int32_t messageDestination;
	/**
	 * This is the message that is to be sent.
	 */
	int32_t message;
	/**
	 * This is the checksum which will verify that the message has been properly received.  It is a simple XOR of the words in the message.
	 */
	int32_t xorChecksum;
};

#endif

