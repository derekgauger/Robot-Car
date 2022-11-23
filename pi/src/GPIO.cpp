/**
 * @file GPIO.cpp
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

#include "GPIO.h"
#include <wiringPi.h>
#include <time.h>
#include <pthread.h>
#include "GenericThreadInfo.h"
#include <sys/syscall.h>
#include <unistd.h>

using namespace std;

namespace se3910RPi {

/**
 * This static variable keeps track of the GPIO instances as they are instantiated.
 * It is used to ensure that we can reference a given instance based upon
 * the GPIO number of the given pin.
 */
static GPIO* gpio_instances[NUMBER_OF_GPIO_PINS];
static GenericThreadInfo* threadInstances[NUMBER_OF_GPIO_PINS];

/**
 * This is a private method that will be used to start a callback handler.
 * @param value This is a void pointer to the GPIO instance.
 */
static void* startCallbackHander(void *gpioInstance);

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt0();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt1();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt2();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt3();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt4();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt5();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt6();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt7();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt8();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt9();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt10();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt11();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt12();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt13();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt14();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt15();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt16();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt17();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt18();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt19();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt20();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt21();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt22();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt23();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt24();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt25();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt26();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt27();

/**
 * This method indicates that a change has occurred on the given pin.
 */
static void myInterrupt28();

static std::mutex cmtx;

/**
 * This will initialize the initialzed variable to 0 until a constructor is invoked.
 */
int GPIO::initialized = 0;

/**
 * This method will start a thread that will serve as the callback handler thread.
 * @param value This is a void pointer to the GPIO instance.
 */
void* startCallbackHander(void *gpioInstance) {

	GPIO *gpio = static_cast<GPIO*>(gpioInstance);
	gpio->callBackThreadHandler();
	return 0;
}

/**
 * This is the constructor for an instance of a GPIO class.
 * @param number This is the Raspberry Pi GPIO Number.
 * @param dir This is the direction for the GPIO pin, either input or
 * output.
 * @param val This is the default value for the GPIO pin.
 */
GPIO::GPIO(int number, DIRECTION dir, VALUE val) {
	std::unique_lock<std::mutex> lck(cmtx);
	// If this is the first time that a constructor has been invoked, initialize
	// the wiringPi devices and setup the pointers.
	if (GPIO::initialized == 0) {
		for (int index = 0; index < NUMBER_OF_GPIO_PINS; index++) {
			gpio_instances[index] = NULL;
		}
		wiringPiSetupGpio();
		GPIO::initialized++;
	}

	// Set the number accordingly, the reference to this class,
	// and callback to be nothing for now.
	this->number = number;
	gpio_instances[number] = this;
	methodCallback = NULL;

	// Initialize the timestamps accordingly.
	isrRisingTimestamp.tv_nsec = 0;
	isrRisingTimestamp.tv_sec = 0;
	isrFallingTimestamp.tv_nsec = 0;
	isrFallingTimestamp.tv_sec = 0;

	// Based upon the GPIO direction, setup the pin accordingly.
	if (dir == GPIO::DIRECTION::GPIO_OUT) {
		pinMode(number, OUTPUT);
		if (val == GPIO::VALUE::GPIO_LOW) {
			digitalWrite(number, LOW);
		} else {
			digitalWrite(number, HIGH);
		}
	} else {
		pinMode(number, INPUT);
	}
	
}

GPIO::~GPIO() {
	// Remove the reference to this instance before it goes away.
	gpio_instances[number] = NULL;
}

/**
 * This method will drive an output pin to the appropriate value.
 * @param This is the value that is to be written to the pin.
 */
void GPIO::setValue(GPIO::VALUE val) {
	if (val == GPIO::VALUE::GPIO_LOW) {
		digitalWrite(number, LOW);
	} else {
		digitalWrite(number, HIGH);
	}
}

/**
 * This method will return the current state of this GPIO pin.
 * @return The return will be high or low depending on the state of the pin.
 */
GPIO::VALUE GPIO::getValue() {
	if (digitalRead(number) == HIGH) {
		return GPIO::GPIO_HIGH;
	} else {
		return GPIO::GPIO::GPIO_LOW;
	}
}

/**
 * This method will enable an interrupt to be processed based upon edges.
 * In doing this, callbacks can be made based upon the GPIO pin changing
 * state and methods can block for IO purposes.
 * @param edgeType This is the edge type that is to be processed as an
 * interrupt.
 */
void GPIO::enableEdgeInterrupt(EDGE edgeType) {
	// This will determine what type of response is expected.
	int responsiveEdges;

	if (edgeType == GPIO_RISING) {
		responsiveEdges = INT_EDGE_RISING;
	} else if (edgeType == GPIO_FALLING) {
		responsiveEdges = INT_EDGE_FALLING;
	} else if (edgeType == GPIO_BOTH) {
		responsiveEdges = INT_EDGE_BOTH;
	} else {
		responsiveEdges = INT_EDGE_SETUP;
	}

	// The following code is ugly, but it sets up the method to be invoked
	// when an edge transition occurs. There is a much cleaner way of doing
	// this with table entries and function pointers.
	switch (this->number) {
	case 0:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt0);
		break;
	case 1:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt1);
		break;
	case 2:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt2);
		break;
	case 3:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt3);
		break;
	case 4:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt4);
		break;
	case 5:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt5);
		break;
	case 6:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt6);
		break;
	case 7:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt7);
		break;
	case 8:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt8);
		break;
	case 9:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt9);
		break;
	case 10:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt10);
		break;
	case 11:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt11);
		break;
	case 12:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt12);
		break;
	case 13:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt13);
		break;
	case 14:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt14);
		break;
	case 15:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt15);
		break;
	case 16:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt16);
		break;
	case 17:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt17);
		break;
	case 18:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt18);
		break;
	case 19:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt19);
		break;
	case 20:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt20);
		break;
	case 21:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt21);
		break;
	case 22:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt22);
		break;
	case 23:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt23);
		break;
	case 24:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt24);
		break;
	case 25:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt25);
		break;
	case 26:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt26);
		break;
	case 27:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt27);
		break;
	case 28:
		wiringPiISR(this->number, responsiveEdges, &myInterrupt28);
		break;
	}
}

/**
 * This method will block the calling thread for up to maxWait ms unless
 * an edge occurs.
 * @param maxWait This is the maximum blocked period in ms.
 * @return 0 if the edge occurred or -1 if a timeout occurred.
 */
int GPIO::waitForEdge(int maxWait) // waits until button is pressed up to a maximum value.
		{
	std::unique_lock<std::mutex> lck(mtx);

	if (std::cv_status::timeout
			== cv.wait_for(lck, std::chrono::milliseconds(maxWait))) {
		// A timeout occurred.  Return -1.
		return -1;
	} else {
		// An edge occurred.
		return 0;
	}
}

/**
 * This method will obtain the timestamp from the last time the pin
 * transitioned high.
 * @return The timestamp of the transition will be returned.
 */
struct timespec GPIO::getRisingISRTimestamp() {
	return isrRisingTimestamp;
}

/**
 * This method will obtain the timestamp from the last time the pin
 * transitioned low.
 * @return The timestamp of the transition will be returned.
 */
struct timespec GPIO::getFallingISRTimestamp() {
	return isrFallingTimestamp;
}


/**
 * This method will set the callback handler.  If this is NULL, no method
 * will be invoked.  However, if it is non-NULL and the class triggers on
 * edges, the method described here will be called when an edge transition
 * occurs.
 * @param methodCallback This is a function pointer that is to be used on
 * the callback.
 */
void GPIO::setCallbackMethod(CallbackType methodCallback) {
	// To begin, if the method callback is currently NULL, we will need to
	// start the thread.
	// Otherwise, if it is not NULL and we are NULLing it.
	// If it is not NULL and we are simply changing the method, then we do
	// not need to worry about anything other than changing the method.
	pthread_t thread;
	if (this->methodCallback == NULL) {
		this->methodCallback = methodCallback;

		int policy;
		struct sched_param sparam;

		// TODO This code is intended to start the new thread that will make
		// the callback at the same priority as the calling thread.
		// However, that is not working right now on the Pi, so this needs
		// Some improvement.

		pthread_attr_t tattr;
		pthread_attr_init(&tattr);
		pthread_getschedparam(pthread_self(), &policy, &sparam);

		pthread_create(&thread, &tattr, &startCallbackHander,
				static_cast<void*>(this));
		pthread_setschedparam(thread, policy, &sparam);
	} else {
		// This will handle the case of setting it to a new method and / or stopping the callback.
		this->methodCallback = methodCallback;
	}
}

/**
 * This method is used by the friend method to handle callback methods.
 * It should never be invoked from outside of the class.  In essence,
 * what it will do a loop so long as the callback method is not NULL,
 * invoking the callback method whenever an edge transitions.
 */
void GPIO::callBackThreadHandler() {
	while (this->methodCallback != NULL) {

		std::unique_lock<std::mutex> lck(mtx);
		ccv.wait(lck);
		this->methodCallback(0);
	}
}

/**
 * This method will be invoked if an interrupt is enabled whenever a
 * transition in pin value occurs.
 */
void GPIO::handleInterruptDrivenGPIOPin() {
	if (digitalRead(number) == HIGH) {
		// Update the timestamp for the interrupt.
		clock_gettime(CLOCK_REALTIME, &isrRisingTimestamp);
	} else {
		// Update the timestamp for the interrupt.
		clock_gettime(CLOCK_REALTIME, &isrFallingTimestamp);
	}
	// Notify the condition variable for a blocked call.
	cv.notify_one();
	// Notify the condition variable for the callback.
	ccv.notify_one();
}

/**
 * This method will obtain the GPIO Pin number associated with this instance of the GPIO class.
 * @return The unique pin number identifying the pin will be returned.
 */
int GPIO::getPinNumber() {
	return this->number;
}

/**
 * This method, and the following methods, will be invoked as ISR's when an edge transitions if edge transitions are enabled.
 */
void myInterrupt0(void) {
	gpio_instances[0]->handleInterruptDrivenGPIOPin();
	if (threadInstances[0]==NULL)
	{
		threadInstances[0] = new GenericThreadInfo("GPIO 0", syscall(SYS_gettid));
	}
}

void myInterrupt1(void) {
	gpio_instances[1]->handleInterruptDrivenGPIOPin();
	if (threadInstances[1]==NULL)
	{
		threadInstances[1] = new GenericThreadInfo("GPIO 1", syscall(SYS_gettid));
	}

}
void myInterrupt2(void) {
	gpio_instances[2]->handleInterruptDrivenGPIOPin();
	if (threadInstances[2]==NULL)
	{
		threadInstances[2] = new GenericThreadInfo("GPIO 2", syscall(SYS_gettid));
	}

}
void myInterrupt3(void) {
	gpio_instances[3]->handleInterruptDrivenGPIOPin();
	if (threadInstances[3]==NULL)
	{
		threadInstances[3] = new GenericThreadInfo("GPIO 3", syscall(SYS_gettid));
	}


}
void myInterrupt4(void) {
	gpio_instances[4]->handleInterruptDrivenGPIOPin();
	if (threadInstances[4]==NULL)
	{
		threadInstances[4] = new GenericThreadInfo("GPIO 4", syscall(SYS_gettid));
	}


}
void myInterrupt5(void) {
	gpio_instances[5]->handleInterruptDrivenGPIOPin();
	if (threadInstances[5]==NULL)
	{
		threadInstances[5] = new GenericThreadInfo("GPIO 5", syscall(SYS_gettid));
	}


}
void myInterrupt6(void) {
	gpio_instances[6]->handleInterruptDrivenGPIOPin();
	if (threadInstances[6]==NULL)
	{
		threadInstances[6] = new GenericThreadInfo("GPIO 6", syscall(SYS_gettid));
	}


}
void myInterrupt7(void) {
	gpio_instances[7]->handleInterruptDrivenGPIOPin();
	if (threadInstances[7]==NULL)
	{
		threadInstances[7] = new GenericThreadInfo("GPIO 7", syscall(SYS_gettid));
	}


}
void myInterrupt8(void) {
	gpio_instances[8]->handleInterruptDrivenGPIOPin();
	if (threadInstances[8]==NULL)
	{
		threadInstances[8] = new GenericThreadInfo("GPIO 8", syscall(SYS_gettid));
	}


}
void myInterrupt9(void) {
	gpio_instances[9]->handleInterruptDrivenGPIOPin();
	if (threadInstances[9]==NULL)
	{
		threadInstances[9] = new GenericThreadInfo("GPIO 9", syscall(SYS_gettid));
	}


}
void myInterrupt10(void) {
	gpio_instances[10]->handleInterruptDrivenGPIOPin();
	if (threadInstances[10]==NULL)
	{
		threadInstances[10] = new GenericThreadInfo("GPIO 10", syscall(SYS_gettid));
	}


}
void myInterrupt11(void) {
	gpio_instances[11]->handleInterruptDrivenGPIOPin();
	if (threadInstances[11]==NULL)
	{
		threadInstances[11] = new GenericThreadInfo("GPIO 11", syscall(SYS_gettid));
	}

}
void myInterrupt12(void) {
	gpio_instances[12]->handleInterruptDrivenGPIOPin();
	if (threadInstances[12]==NULL)
	{
		threadInstances[12] = new GenericThreadInfo("GPIO 12", syscall(SYS_gettid));
	}

}
void myInterrupt13(void) {
	gpio_instances[13]->handleInterruptDrivenGPIOPin();
	if (threadInstances[13]==NULL)
	{
		threadInstances[13] = new GenericThreadInfo("GPIO 13", syscall(SYS_gettid));
	}

}
void myInterrupt14(void) {
	gpio_instances[14]->handleInterruptDrivenGPIOPin();
	if (threadInstances[14]==NULL)
	{
		threadInstances[14] = new GenericThreadInfo("GPIO 14", syscall(SYS_gettid));
	}

}
void myInterrupt15(void) {
	gpio_instances[15]->handleInterruptDrivenGPIOPin();
	if (threadInstances[15]==NULL)
	{
		threadInstances[15] = new GenericThreadInfo("GPIO 15", syscall(SYS_gettid));
	}

}
void myInterrupt16(void) {
	gpio_instances[16]->handleInterruptDrivenGPIOPin();
	if (threadInstances[16]==NULL)
	{
		threadInstances[16] = new GenericThreadInfo("GPIO 16", syscall(SYS_gettid));
	}

}
void myInterrupt17(void) {
	gpio_instances[17]->handleInterruptDrivenGPIOPin();
	if (threadInstances[17]==NULL)
	{
		threadInstances[17] = new GenericThreadInfo("GPIO 17", syscall(SYS_gettid));
	}

}
void myInterrupt18(void) {
	gpio_instances[18]->handleInterruptDrivenGPIOPin();
	if (threadInstances[18]==NULL)
	{
		threadInstances[18] = new GenericThreadInfo("GPIO 18", syscall(SYS_gettid));
	}

}
void myInterrupt19(void) {
	gpio_instances[19]->handleInterruptDrivenGPIOPin();
	if (threadInstances[19]==NULL)
	{
		threadInstances[19] = new GenericThreadInfo("GPIO 19", syscall(SYS_gettid));
	}

}
void myInterrupt20(void) {
	gpio_instances[20]->handleInterruptDrivenGPIOPin();
	if (threadInstances[20]==NULL)
	{
		threadInstances[20] = new GenericThreadInfo("GPIO 20", syscall(SYS_gettid));
	}

}
void myInterrupt21(void) {
	gpio_instances[21]->handleInterruptDrivenGPIOPin();
	if (threadInstances[21]==NULL)
	{
		threadInstances[21] = new GenericThreadInfo("GPIO 21", syscall(SYS_gettid));
	}

}
void myInterrupt22(void) {
	gpio_instances[22]->handleInterruptDrivenGPIOPin();
	if (threadInstances[22]==NULL)
	{
		threadInstances[22] = new GenericThreadInfo("GPIO 22", syscall(SYS_gettid));
	}

}
void myInterrupt23(void) {
	gpio_instances[23]->handleInterruptDrivenGPIOPin();
	if (threadInstances[23]==NULL)
	{
		threadInstances[23] = new GenericThreadInfo("GPIO 23", syscall(SYS_gettid));
	}

}
void myInterrupt24(void) {
	gpio_instances[24]->handleInterruptDrivenGPIOPin();
	if (threadInstances[24]==NULL)
	{
		threadInstances[24] = new GenericThreadInfo("GPIO 24", syscall(SYS_gettid));
	}

}
void myInterrupt25(void) {
	gpio_instances[25]->handleInterruptDrivenGPIOPin();
	if (threadInstances[25]==NULL)
	{
		threadInstances[25] = new GenericThreadInfo("GPIO 25", syscall(SYS_gettid));
	}

}
void myInterrupt26(void) {
	gpio_instances[26]->handleInterruptDrivenGPIOPin();
	if (threadInstances[26]==NULL)
	{
		threadInstances[26] = new GenericThreadInfo("GPIO 26", syscall(SYS_gettid));
	}

}
void myInterrupt27(void) {
	gpio_instances[27]->handleInterruptDrivenGPIOPin();
	if (threadInstances[27]==NULL)
	{
		threadInstances[27] = new GenericThreadInfo("GPIO 27", syscall(SYS_gettid));
	}

}
void myInterrupt28(void) {
	gpio_instances[28]->handleInterruptDrivenGPIOPin();
	if (threadInstances[28]==NULL)
	{
		threadInstances[28] = new GenericThreadInfo("GPIO 28", syscall(SYS_gettid));
	}

}

} /* namespace exploringRPi */
