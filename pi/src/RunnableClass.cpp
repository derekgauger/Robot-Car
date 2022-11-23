/**
 * @file RunnableClass.cpp
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
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
 * This file contains a set of wrapper classes to make multithreading in C++
 * a bit more like the Java interfaces taught in SE2811.  The runnable class
 * mimics the main functionality of the Java Runnable Interface.
 */

#include "RunnableClass.h"
#include <thread>
#include <string>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

/*
 * This is a file scopes variable which holds a list of the threads that are running.
 */
std::list<RunnableClass*> RunnableClass::runningThreads;

/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 */
RunnableClass::RunnableClass(std::string threadName) {
	priority = 1;

	// Set the name of the thread accordingly.
	myName = threadName;

	// Add the thread to the list of threads that are executing.
	runningThreads.push_front(this);
}

/**
 * This method will return the CPU usage for the given task.  The usage will be as a percentage value.
 */
double RunnableClass::getCPUUsageInfo()
{
	/**
	 * Note that for a simple runnable class, we do not currently have this metric.
	 */
	return 0.0;
}

/**
 * This method will print out to the console each of the running threads and their thread ID's.
 */
void RunnableClass::printThreads() {
	std::cout
			<< "===============================================================================================\nThread Diagnostic Information:\n";
	// Print the header out
	std::cout << "Thread\tTask              \tPrio.\tperiod(us)\tLast Execution(us)\tWCET(us)\tLast Wall Time(us)\tWCWT(us)\tCPU Usage\n";
	double totalCPUUsage = 0.0;
	for (std::list<RunnableClass*>::iterator it = runningThreads.begin();
			it != runningThreads.end(); it++) {
		RunnableClass *rc = *it;
		rc->printInformation();
		totalCPUUsage += rc->getCPUUsageInfo();
	}
	std::cout<< "Total CPU Usage: "<< std::fixed << std::setprecision(3) << totalCPUUsage << "\n";
	std::cout<< "===============================================================================================\n";

}

/**
	 * This method will obtain the thread ID for this runnable.
	 * @return The OS Thread ID.
	 */
	pid_t RunnableClass::getThreadID()
	{
		return this->myOSThreadID;
	}


/**
 * This method will reset the thread information which is dynamic in nature and changes as the robot runs.
 * This predominantly impacts threads which are not part of the Runnable class.
 */
void RunnableClass::resetAllThreadInformation() {
	for (std::list<RunnableClass*>::iterator it = runningThreads.begin();
			it != runningThreads.end(); it++) {
		RunnableClass *rc = *it;
		rc->resetThreadDiagnostics();
	}
}

/**
 * This method will reset thread diagnostics back to their default values.
 */
void RunnableClass::resetThreadDiagnostics() {
	// For a Runnable class, this method does nothing, as there currently are no diagnostics for runnable threads.
}

/**
 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
 */
void RunnableClass::printInformation() {

	std::cout << myOSThreadID << "\t" << std::setw(18) << myName << "\t "
			<< std::setw(5) << getPriority() << "\n ";
}

/*
 * This is the default destructor for the class.  It must properly clean up the instantiated thread.
 */
RunnableClass::~RunnableClass() {
	/**
	 * If the thread is not NULL, delete it.
	 */
	if (myThread != NULL) {
		delete myThread;
	}
}

/**
 * This private method initializes the runnable class.  It is actually the method invoked when the thread starts.
 */
void RunnableClass::invokeRunMethod() {
	// Setup the operating thread to be a real time thread.
	struct sched_param p;

	runStarted = true;
	runCompleted = false;

	if (priority > 0) {

		if (priority > sched_get_priority_max(SCHED_FIFO)) {
			p.__sched_priority = sched_get_priority_max(SCHED_FIFO);
		} else {
			p.__sched_priority = priority;
		}

		if (sched_setscheduler(0, SCHED_FIFO, &p) != 0) {
			printf("Failed to set the scheduler\n");
		}
	}

	// Obtain the thread id by making a system call.
	myOSThreadID = syscall(SYS_gettid);

	// Now invoke the run method,
	this->run();

	// When run returns, indicate that the run is completed.
	runCompleted = true;
	runStarted = false;
}

/**
 * This virtual method is the start method.  It must be implemented in child classes.  The purpose of this method is to instantiate a new thread and invoke the run method.
 * This method will cause the task to start with the default priority.
 */
void RunnableClass::start() {
	keepGoing = true;
	startChildRunnables();
	myThread = new std::thread(&RunnableClass::invokeRunMethod, this);
}

/**
 * This virtual method is the start method.  It must be implemented in child classes.  The purpose of this method is to instantiate a new thread and invoke the run method.
 * @param priority This is the priority for the task.  It must be between 0 and 99, with 99 being the highest priority.
 */
void RunnableClass::start(int priority) {
	this->priority = priority;
	this->start();
}

/**
 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 *
 */
void RunnableClass::startChildRunnables() {
	/**
	 * This default implementation does nothing.
	 */
}

/**
 * This method will determine if the given runnable class has been started or not.
 * @return true if the task has been started.  False otherwise.
 */
bool RunnableClass::isStarted() {
	return runStarted;
}

/**
 * This method will stop the execution of the given class.
 */
void RunnableClass::stop() {
	/**
	 * Stop the thread by changing the value of keepGoing.  If there is a child class that has a RUnnable object inside of it, then stop must be called on that object.
	 */
	keepGoing = false;
}

/**
 * This method will determine if the given thread is shutdown.
 * @return The return will be true if the thread is shutdown or false otherwise.
 */
bool RunnableClass::isShutdown() {
	return runCompleted;
}

/**
 * This method will block waiting for the given thread to terminate before continuing.
 * If a class that is derived from this class has a child class with a RUnnable object,
 * this method must be overridden to wait for the child method to shutdown.
 */
void RunnableClass::waitForShutdown() {
	if (myThread != NULL) {
		/**
		 * Join the thread and wait for shutdown.
		 */
		myThread->join();
	}
}

/**
 * This method will set the priority for the given task.
 * @param priority This is the priority for the task.  It must be between 0 and 99, with 99 being the highest priority.
 */
void RunnableClass::setPriority(int priority) {
	if (priority >= 0 && priority <= 100) {
		this->priority = priority;
	}
}

/**
 * This method will obtain the priority set for this runnable class.
 * @return The return will be the set priority for the runnable class.
 */
int RunnableClass::getPriority() {
	return this->priority;
}
