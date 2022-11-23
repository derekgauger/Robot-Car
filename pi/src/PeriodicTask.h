/**
 * @file PeriodicTask.h
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

#ifndef PERIODICTASK_H_
#define PERIODICTASK_H_

#include "RunnableClass.h"

#include <chrono>

class PeriodicTask: public RunnableClass {
private:
	/**
	 * This variable sets the period for the task.  The period defines the length of
	 * time from one invocation until the next invocation.  The task period is given
	 * in microseconds.  Default value is 100 ms or 100000 microseconds.
	 */
	uint32_t taskPeriod = 100000;

	/**
	 * This variable stores the worst case execution time from a CPU time standpoint.
	 */
	long worstCaseExecutionTime = 0;

	/**
	 * This variable holds the last execution value for the CPU, showing the time it took to execute.
	 */
	long lastExecutionTime = 0;

	/**
	 * This variable holds the last wall time measurement for the given periodic task.  The wall time is the time from the start to the end of the task running.
	 */
	std::chrono::microseconds lastWallTime = std::chrono::microseconds(0);

	/**
	 * This variable holds the worst case wall time.  This is the worst case from start to finish of the given periodic task.
	 */
	std::chrono::microseconds worstCaseWallTime = std::chrono::microseconds(0);

	/**
	 * This is a private method that will be used by start to invoke the run method.
	 */
	void invokeRun();

	/**
	 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
	 */
	void waitForNextExecution(std::chrono::microseconds remainingSleepTime);
	/**
	 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
	 */
	void waitForNextExecution();

public:
	/**
	 * This is the default constructor for the class.
	 * @param threadName This is the name of the thread in a human readable format.
	 * @param period This is the period for the task, given in microseconds.
	 * Must be at least 100 microseconds.
	 */
	PeriodicTask(std::string threadName, uint32_t period);

	/**
	 * This method will clean up from the periodic task and its execution.
	 */
	virtual ~PeriodicTask();

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time.
	 */
	virtual void taskMethod()=0;

	/**
	 * This will set the period for the given task.
	 * @param period This is the period, given in microseconds, between executions of this task.
	 * However, it must be at least 100 microseconds (.1 ms)
	 */
	virtual void setTaskPeriod(uint32_t period) final;

	/**
	 * This method will return the task period.
	 * @return the Task period in microseconds will be returned.
	 */
	virtual uint32_t getTaskPeriod() final;

	/**
	 * This is the run method for the class.
	 */
	void run() final;

	/**
	 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
	 */
	virtual void printInformation();

	virtual double getCPUUsageInfo();

	/**
	 * This method will reset thread diagnostics back to their default values.  The wall times and CPU times will be set to 0.
	 */
	virtual void resetThreadDiagnostics();

};

#endif /* PERIODICTASK_H_ */
