/**
 * @file RunnableClass.h
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

#ifndef RUNNABLECLASS_H_
#define RUNNABLECLASS_H_

#include <thread>
#include <string>
#include <list>
#include <sys/types.h>

/**
 * This is the runnable class, which mimics the runnable interface from Java.  It is a virtual class which should not directly be instantiated.
 */
class RunnableClass {
protected:
	/**
	 * This is a list of all of the running threads which have been started by this set of libraries.
	 */
	static std::list<RunnableClass*> runningThreads;

	/**
	 * This is the instance of the thread that is to be executed by this class.  AT instantiation it is NULL.  It will be instantiated when the start method of the class is invoked.
	 */
	std::thread *myThread = NULL;

	/**
	 * This is the thread id (tid) for this task.
	 */
	pid_t myOSThreadID = 0;

	/**
	 * This is the name of the thread.  It is shown as a string so that it can easily be viewed in human-readable format.
	 */
	std::string myName;

	/**
	 * This variable will determine whether or not the given thread is to continue executing.
	 */
	bool keepGoing = true;

	/**
	 * This variable will determine if the current Runnable class has completed execution and the run method has returned.
	 */
	bool runCompleted = false;

	/**
	 * This is the priority of the given thread.  A value of -1 indicates no change in the priority and to use the default setup.
	 * For a runnable class, this must be set before the class starts.  For other derived classes, it may be changeable at run time.
	 */
	int priority = -1;

	/**
	 * This variable will determine whether or not the current task has been started or not.
	 */
	bool runStarted = false;

private:
	/**
	 * This private method initializes the runnable class.  It is actually the method invoked when the thread starts, and it will ultimately call the Run method.
	 */
	virtual void invokeRunMethod() final;

public:
	/**
	 * This method will print out to the console each of the running threads and their thread ID's.
	 */
	static void printThreads();

	/**
	 * This method will reset the thread information which is dynamic in nature and changes as the robot runs.
	 * This predominantly impacts threads which are not part of the Runnable class.
	 */
	static void resetAllThreadInformation();

	/**
	 * This method will reset thread diagnostics back to their default values.
	 */
	virtual void resetThreadDiagnostics();

	/**
	 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
	 */
	virtual void printInformation();

	/**
	 * This method will return the CPU usage for the given task.  The usage will be as a percentage value.
	 */
	virtual double getCPUUsageInfo();

	/**
	 * This method will obtain the thread ID for this runnable.
	 * @return The OS Thread ID.
	 */
	virtual pid_t getThreadID();

	/**
	 * This is the default constructor for the class.
	 * @param threadName This is the name of the thread in a human readable format.
	 */
	RunnableClass(std::string threadName);

	/**
	 * This is the default destructor for the class.  It must properly clean up the instantiated thread.
	 */
	virtual ~RunnableClass();

	/**
	 * This virtual method is the start method.  It must be implemented in child classes.  The purpose of this method is to instantiate a new thread and invoke the run method.
	 * This method will cause the task to start with the default priority.
	 */
	virtual void start() final;

	/**
	 * This virtual method is the start method.  It must be implemented in child classes.  The purpose of this method is to instantiate a new thread and invoke the run method.
	 * @param priority This is the priority for the task.  It must be between 0 and 99, with 99 being the highest priority.
	 */
	virtual void start(int priority) final;


	/**
	 * This method will set the priority for the given task, using the real time FIFO scheduler as well as setting the priority.
	 * @param priority This is the priority for the task.  It must be between 0 and 99, with 99 being the highest priority.
	 */
	virtual void setPriority(int priority) final;

	/**
	 * This method will obtain the priority set for this runnable class.
	 * @return The return will be the set priority for the runnable class.
	 */
	virtual int getPriority() final;

	/**
	 * This is the virtual run method.  It will execute the given code that is to be executed by this class.
	 */
	virtual void run()=0;

	/**
	 * This method will determine if the given thread is shutdown.
	 * @return The return will be true if the thread is shutdown or false otherwise.
	 */
	virtual bool isShutdown() final;


	/**
	 * This method will determine if the given runnable class has been started or not.
	 * @return true if the task has been started.  False otherwise.
	 */
	virtual bool isStarted() final;

	/**
	 * This method will stop the execution of the given class.
	 */
	virtual void stop() ;

	/**
	 * This method will block waiting for the given thread to terminate before continuing.  This should be overridden if any child class
	 * has it's own runnable objects encapsulated within it.
	 *
	 */
	virtual void waitForShutdown();

	/**
	 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
	 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
	 * contains an instance of a runnable object, then this method must be overridden in the derived class.
	 *
	 */
	virtual void startChildRunnables();

};

#endif /* RUNNABLECLASS_H_ */
