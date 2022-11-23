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
	 *      This file defines the behavior for a generic thread.  This is a thread that is part of the system but is not actually started by program.periodic task.  A periodic task is
	 *      one in which the run method is called periodically by the task manager.
	 */

#ifndef GENERICTHREADINFO_H_
#define GENERICTHREADINFO_H_

	#include "RunnableClass.h"
	#include <chrono>

class GenericThreadInfo : public RunnableClass {
	private:

	public:
		/**
		 * This is the default constructor for the class.
		 * @param threadName This is the name of the thread in a human readable format.
		 */
	GenericThreadInfo(std::string threadName, pid_t myThreadID);

		/**
		 * This method will clean up from the periodic task and its execution.
		 */
		virtual ~GenericThreadInfo();

		/**
		 * This is the run method for the class.
		 */
		void run() final;

};

#endif /* GENERICTHREADINFO_H_ */
