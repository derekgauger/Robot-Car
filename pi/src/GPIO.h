/**
 * @file GPIO.h
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
 * This file contains a set of GPIO wrapper classes for the Raspberry Pi.
 * This class allows a user
 * of the Raspberry PI to readily interface with the wiringPi libraries in an
 * object oriented fashion in C++, invoking callbacks, and writing C++ using
 * objects.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#define NUMBER_OF_GPIO_PINS (28)

namespace se3910RPi {

typedef int (*CallbackType)(int);

/**
 * @class GPIO
 * @brief GPIO GPIO wrapper classes for usage with C++ and wiringPi.
 */
class GPIO {
public:
	/**
	 * This enumeration sets up the pin to either be an input or an output pin.
	 */
	enum DIRECTION {
		GPIO_IN, GPIO_OUT
	};
	/**
	 * This enumeration defines the values on the pin as being either high or
	 * low.
	 */
	enum VALUE {
		GPIO_LOW = 0, GPIO_HIGH = 1
	};
	/**
	 * This enumeration is used to define whether an ISR is fired based on
	 * transitions in pin values.
	 */
	enum EDGE {
		GPIO_NONE, GPIO_RISING, GPIO_FALLING, GPIO_BOTH
	};

private:
	/**
	 * This attribute indicates whether any GPIO has been instantiated.
	 * The first GPIO to be instantiated will also setup the wiringPi devices.
	 * Subsequent instantiations will not do so.
	 */
	static int initialized;

	/**
	 * This is the GPIO pin number for the given pin.
	 */
	int number;

	/**
	 * This is a mutex that is used to protect critical resources within this
	 * class.
	 */
	std::mutex mtx;
	
	/**
	 * These two condition variables are used internally to allow the
	 * invocation of a callback method and a block for edge method.
	 */
	std::condition_variable cv;
	std::condition_variable ccv;

	/**
	 * This is a callback variable that is used to invoke a method when a
	 * transition in the GPIO value occurs.
	 */
	CallbackType methodCallback;

	/**
	 * This is a timestamp marked when a rising edge is detected within the
	 * system, if the system is setup to handle edges.
	 */
	struct timespec isrRisingTimestamp;

	/**
	 * This is a timestamp marked when a falling edge is detected within the
	 * system, if the system is setup to handle edges.
	 */
	struct timespec isrFallingTimestamp;



public:
	/**
	 * This is the constructor for an instance of a GPIO class.
	 * @param number This is the Raspberry Pi GPIO Number.
	 * @param dir This is the direction for the GPIO pin, either input or
	 * output.
	 * @param val This is the default value for the GPIO pin.
	 */
	GPIO(int number, DIRECTION dir, VALUE val = GPIO_LOW);

	/**
	 * This is the destructor for the class, which will clean up the existing
	 * instance.
	 */
	virtual ~GPIO();  //destructor will unexport the pin

	// General Input and Output Settings
	/**
	 * This method will drive an output pin to the appropriate value.
	 * @param This is the value that is to be written to the pin.
	 */
	virtual void setValue(GPIO::VALUE);

	/**
	 * This method will return the current state of this GPIO pin.
	 * @return The return will be high or low depending on the state of the pin.
	 */
	virtual GPIO::VALUE getValue();

// Edge and Interrupt Methods

	/**
	 * This method will enable an interrupt to be processed based upon edges.
	 * In doing this, callbacks can be made based upon the GPIO pin changing
	 * state and methods can block for IO purposes.
	 * @param edgeType This is the edge type that is to be processed as an
	 * interrupt.
	 */
	virtual void enableEdgeInterrupt(EDGE edgeType);

	/**
	 * This method will block the calling thread for up to maxWait ms unless
	 * an edge occurs.  Note that this should only be called once at a time,
	 * as multiple invocations will result in non-deterministic behavior.
	 * @param maxWait This is the maximum blocked period in ms.
	 * @return 0 if the edge occurred or -1 if a timeout occurred.
	 */
	virtual int waitForEdge(int maxWait);

	/**
	 * This method will obtain the timestamp from the last time the pin
	 * transitioned high.
	 * @return The timestamp of the transition will be returned.
	 */
	virtual struct timespec getRisingISRTimestamp();

	/**
	 * This method will obtain the timestamp from the last time the pin
	 * transitioned low.
	 * @return The timestamp of the transition will be returned.
	 */
	virtual struct timespec getFallingISRTimestamp();

	/**
	 * This method will set the callback handler.  If this is NULL, no method
	 * will be invoked.  However, if it is non-NULL and the class triggers on
	 * edges, the method described here will be called when an edge transition
	 * occurs.
	 * @param methodCallback This is a function pointer that is to be used on
	 * the callback.
	 */
	virtual void setCallbackMethod(CallbackType methodCallback);

	/**
	 * This method is used by the friend method to handle callback methods.
	 * It should never be invoked from outside of the class.  In essence,
	 * what it will do a loop so long as the callback method is not NULL,
	 * invoking the callback method whenever an edge transitions.
	 */
	virtual void callBackThreadHandler();

	/**
	 * This method will be invoked if an interrupt is enabled whenever a
	 * transition in pin value occurs.
	 */
	virtual void handleInterruptDrivenGPIOPin();

	/**
	 * This method will obtain the GPIO Pin number associated with this instance of the GPIO class.
	 * @return The unique pin number identifying the pin will be returned.
	 */
	virtual int getPinNumber();



};

} /* namespace se3910RPi */

#endif /* GPIO_H_ */
