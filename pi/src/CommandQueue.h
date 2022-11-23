/**
 * @file CommandQueue.h
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
 * Function prototypes and class definitions for the command queue class.
 * This class, the CommandQueue, allows a user to enque a set of commands for a device. The commands must be sinple integers,
 * but they can have bitmapped representations.  The queue can hold as many items as is necessary.
 *
 * @author Walter Schilling (schilling@msoe.edu)
 * @bug No known bugs.
 */

#ifndef COMMANDQUEUE_H_
#define COMMANDQUEUE_H_
#include <queue>        /* Required for the queue data structure.*/
#include <mutex>        /* Required for locking critical sections. */
#include <semaphore.h>  /* required for semaphores */

class CommandQueue {
private:
	/**
	 * This attribute holds the contents of the queue.  Items are processed in FIFO order.
	 */
	std::queue<int> commandQueueContents;
	/**
	 * This is a counting semaphore which keeps track of how many items are on the queue.
	 */
	sem_t queueCountSemaphore;
	/**
	 * This is a mutex for the class which is used to lock critical sections in different methods.
	 */
	std::mutex queueMutex;

public:
	/**
	 * This is the default constructor, which creates an instance of the queue.
	 */
	CommandQueue();

	/**
	 * The virtual destructor, which deallocates any allocated resources.
	 */
	virtual ~CommandQueue();

	/**
	 * This method will indicate whether or not the queue has an item that is ready to be dequeued.
	 * @return true if there is an item on the queue.  False otherwise.
	 */
	bool hasItem();

	/**
	 * This method will dequeue the next command from the queue.  This method will block if there are no items on the queue.
	 * @return The return will be the next command that is to be processed.
	 */
	int dequeue();

	/**
	 * This method will enqueue a command on the queue.  Any number can be enqueued as a command.
	 * Enqueueing a command will cause a thread blocked waiting for a command to be unblocked.
	 * @param value This is the value that is to be enqueued.
	 */
	void enqueue(int value);
};

#endif /* COMMANDQUEUE_H_ */
