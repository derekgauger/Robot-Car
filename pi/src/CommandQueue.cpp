/**
* Lab 4 - Networked Lights
* @file CommandQueue.cpp
* @author Derek Gauger and Hannah Rindfleisch
* @version 1.0
**/

#include "CommandQueue.h"
#include <queue>
#include <mutex>
#include <semaphore.h>

//using namespace se3910RPi;

/**
 * This is the default constructor, which creates an instance of the queue.
 */
CommandQueue::CommandQueue() {
	sem_init(&queueCountSemaphore, 0, 0);
}

/**
 * The virtual destructor, which deallocates any allocated resources.
 */
CommandQueue::~CommandQueue() {
	sem_destroy(&queueCountSemaphore);
}

/**
 * This method will indicate whether or not the queue has an item that is ready to be dequeued.
 * @return true if there is an item on the queue.  False otherwise.
 */
bool CommandQueue::hasItem() {
	return !commandQueueContents.empty();
}

/**
 * This method will dequeue the next command from the queue.  This method will block if there are no items on the queue.
 * @return The return will be the next command that is to be processed.
 */
int CommandQueue::dequeue() {
	sem_wait(&queueCountSemaphore);
	queueMutex.lock();
	int retValue = commandQueueContents.front();
	commandQueueContents.pop();
	queueMutex.unlock();

	return retValue;
}

/**
 * This method will enqueue a command on the queue.  Any number can be enqueued as a command.
 * Enqueueing a command will cause a thread blocked waiting for a command to be unblocked.
 * @param value This is the value that is to be enqueued.
 */
void CommandQueue::enqueue(int value) {
	queueMutex.lock();
	commandQueueContents.push(value);
	sem_post(&queueCountSemaphore);
	queueMutex.unlock();
}
