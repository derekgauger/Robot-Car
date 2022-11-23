#include "GenericThreadInfo.h"
#include <sys/resource.h>

/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 */
GenericThreadInfo::GenericThreadInfo(std::string threadName, pid_t myThreadID) :
		RunnableClass(threadName) {
	// Obtain the thread id by making a system call.
	myOSThreadID = myThreadID;
	int which = PRIO_PROCESS;
	this->priority = getpriority(which, myThreadID);
}

/**
 * This method will clean up from the periodic task and its execution.
 */
GenericThreadInfo::~GenericThreadInfo() {
	// Nothing to do in the destructor.

}

/**
 * This is the run method for the class.
 */
void GenericThreadInfo::run() {
	// DO nothing, as this really is just for placeholder purposes.
}

