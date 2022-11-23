/**
 * @file PeriodicTask.cpp
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
 *      This file defines the behavior for a periodic task.  A periodic task is
 *      one in which the run method is called periodically by the task manager.
 */

#include "PeriodicTask.h"
#include <iostream>
#include <chrono>
#include <iomanip>

/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 * @param period This is the period for the task, given in microseconds.
 * Must be at least 100 microseconds.
 */
PeriodicTask::PeriodicTask(std::string threadName, uint32_t period) :
		RunnableClass(threadName) {
	this->setTaskPeriod(period);
	this->setPriority(1);
}

/**
 * This method will clean up from the periodic task and its execution.
 */
PeriodicTask::~PeriodicTask() {
	/**
	 * Nothing to be done in the destructor.
	 */
}

/**
 * This will set the period for the given task.
 * @param period This is the period, given in microseconds, between executions of this task.
 * However, it must be at least 100 microseconds (.1 ms)
 */
void PeriodicTask::setTaskPeriod(uint32_t period) {
	/**
	 * Set the task period to the parameter passed in if it is greater than or equal to 100.
	 */
	if (period >= 100) {
		taskPeriod = period;
	}
}

/**
 * This method will return the task period.
 * @return the Task period in microseconds will be returned.
 */
uint32_t PeriodicTask::getTaskPeriod() {
	return taskPeriod;
}

/**
 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
 */
void PeriodicTask::waitForNextExecution(
		std::chrono::microseconds remainingSleepTime) {
	/**
	 * Sleep for the given amount of time.
	 */
	std::this_thread::sleep_for(remainingSleepTime);
}

/**
 * This method will return the CPU usage for the given task.  The usage will be as a percentage value.
 */
double PeriodicTask::getCPUUsageInfo() {
	double cpuUsage = ((double) worstCaseExecutionTime / (double) taskPeriod)
			* 100.0;
	return cpuUsage;
}

/**
 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
 */
void PeriodicTask::printInformation() {
	std::cout << myOSThreadID << "\t" << std::setw(18) << myName << "\t "
			<< std::setw(5) << getPriority() << "\t " << std::setw(10)
			<< taskPeriod << "\t " << std::setw(18) << lastExecutionTime
			<< "\t " << std::setw(8) << worstCaseExecutionTime << "\t "
			<< std::setw(18) << lastWallTime.count() << "\t " << std::setw(8)
			<< worstCaseWallTime.count() << "\t" << std::fixed
			<< std::setprecision(3) << getCPUUsageInfo() << "%";
	if (worstCaseWallTime.count() > taskPeriod) {
		std::cout << "**";
	}
	std::cout << "\n";
}

/**
 * This method will reset thread diagnostics back to their default values.
 */
void PeriodicTask::resetThreadDiagnostics() {
	// Reset all diagnostic variables to zero.
	worstCaseExecutionTime = 0;
	lastExecutionTime = 0;
	lastWallTime = std::chrono::microseconds(0);
	worstCaseWallTime = std::chrono::microseconds(0);
}

/**
 * This is a private method that will be used by start to invoke the run method.
 */
void PeriodicTask::run() {
	/**
	 * Set keep going to be true, to indicate that the thread is to continue running.
	 */
	keepGoing = true;

	while (keepGoing == true) {
		// Get the start time for the given iteration of the task.
		clockid_t threadTimer;
		struct timespec startTs;
		struct timespec endTs;

		/**
		 * The following gets the wall time, for determining next execution time.
		 */
		std::chrono::high_resolution_clock::time_point start =
				std::chrono::high_resolution_clock::now();

		/**
		 * Obtain the cpu time at the start of this periodic task. This is for CPU time measurement.
		 **/
		pthread_getcpuclockid(pthread_self(), &threadTimer);
		clock_gettime(threadTimer, &startTs);

		/**Now run the task.
		 * Call the task method.
		 */
		this->taskMethod();

		/**
		 *Now get the end CPU time entry.
		 **/
		clock_gettime(threadTimer, &endTs);
		long deltaInus = (endTs.tv_sec * 1000000 + endTs.tv_nsec / 1000)
				- (startTs.tv_sec * 1000000 + startTs.tv_nsec / 1000);

		/**
		 * Determine where we are in terms of the worst case execution time.
		 */
		if (deltaInus > worstCaseExecutionTime) {
			worstCaseExecutionTime = deltaInus;
		}
		lastExecutionTime = deltaInus;

		/**
		 * Now figure out exactly what time it is to schedule the next execution.
		 */
		std::chrono::high_resolution_clock::time_point end =
				std::chrono::high_resolution_clock::now();

		// Now figure out the difference.
		std::chrono::microseconds executionTime = std::chrono::duration_cast<
				std::chrono::microseconds>(end - start);

		lastWallTime = executionTime;
		if (lastWallTime > worstCaseWallTime) {
			worstCaseWallTime = lastWallTime;
		}

		/**
		 * Figure out how long to sleep.
		 */
		std::chrono::microseconds remainingSleepTime =
				std::chrono::microseconds(taskPeriod) - executionTime;

		/**
		 * Sleep until the next execution should occur.
		 */
		waitForNextExecution(remainingSleepTime);
	}
}

