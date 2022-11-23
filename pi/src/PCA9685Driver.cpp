/*
 * PCA9685Driver.cpp
 *
 *  Created on: Jul 2, 2018
 *      Author: se3910
 */

#include "PCA9685Driver.h"
#include "pca9685_reg.h"
#include "I2CDevice.h"
#include <chrono>
#include <thread>
#include <math.h>
#include <map>
#include <mutex>              // std::mutex, std::unique_lock

using namespace se3910RPi;

/**
 * This map will hold the mappings between the channels and the instances.
 */
std::map<uint16_t, PCA9685Driver*> PCA9685Driver::deviceMaps;

/**
 * This mutex will protect critical sections of the code that exist and could be harmed by having race conditions occur.
 */
static std::mutex cmtx;

PCA9685Driver* PCA9685Driver::obtainPCA9685Instance(uint16_t deviceID)
{
	/**
	 * Lock the mutex to prevent simultaneous access here.
	 */
	std::unique_lock<std::mutex> lck(cmtx);
	if (deviceMaps.find(deviceID) == deviceMaps.end())
	{
		// There is no instance.  Make one and insert it.
		deviceMaps.insert(std::make_pair(deviceID, new PCA9685Driver(deviceID)));
		deviceMaps[deviceID]->init();
	}
	deviceMaps[deviceID]->count++;
	return deviceMaps[deviceID];
}


PCA9685Driver::PCA9685Driver(uint16_t deviceAddress) {

	this->device= I2CDevice::obtainDeviceInstance(deviceAddress);
	this->address=deviceAddress;
}

PCA9685Driver::~PCA9685Driver() {
	delete device;
}

void PCA9685Driver::init()
{
	this->device->writeRegister(MODE1, 0b00110001); // Setting mode to sleep so we can change the default PWM frequency

	// Wait 1 ms
	std::this_thread::sleep_for(std::chrono::milliseconds((1)));

	// Write the prescale to setup a 20 ms period.
	this->device->writeRegister(PRE_SCALE, 0x79); // Setting mode to sleep so we can change the default PWM frequency

	// Wait 1 ms
	std::this_thread::sleep_for(std::chrono::milliseconds((1)));

	// Set the preferred mode.
	this->device->writeRegister(MODE1, 0xa1); // Setting mode to sleep so we can change the default PWM frequency

	// Wait 1 ms
	std::this_thread::sleep_for(std::chrono::milliseconds((1)));

	// Set the preferred mode.
	this->device->writeRegister(MODE2, 0b00000100); // Set to our prefered mode[Output logic state not inverted, Outputs change on STOP,
													// totem pole structure, When OE = 1 (output drivers not enabled), LEDn = 0]
}


void PCA9685Driver::setPwm(unsigned int channel, uint16_t onTime, uint16_t offTime)
{
	unsigned int startingAddress;

	startingAddress = 0x06 + 4 * channel;

	// Write to the given channel the on and off time.
	this->device->writeRegister(startingAddress, onTime & 0x00FF);
	this->device->writeRegister(startingAddress+1, (onTime>>8) & 0x00FF);
	this->device->writeRegister(startingAddress+2, offTime & 0x00FF);
	this->device->writeRegister(startingAddress+3, (offTime>>8) & 0x00FF);
}
/**
 * This method will set the width of the given pulse.
 * @param width The width is the time the pulse will be on, given in us.
 */
void PCA9685Driver::setDutyCycle(unsigned int channel, uint16_t percent)
{
	uint16_t pulse = (4096 * percent)/1000;
	setPwm(channel, 0, pulse-1);
}

/**
 * This method will set the frequency for the PWM module driven by this device.
 * @freq This is the frequency, given in hertz.
 */
void PCA9685Driver::setFrequency(uint32_t freq)
{
	   double prescaleval = 25000000.0;    // 25MHz
	   prescaleval /= 4096.0;       // 12-bit
	   prescaleval = prescaleval / freq;
	   prescaleval -= 1.0;
	   int prescale = (int)(floor(prescaleval + 0.5));

	   this->device->writeRegister(MODE1, 0b00110001); // Setting mode to sleep so we can change the default PWM frequency

		// Write the prescale to setup a 20 ms period.
		this->device->writeRegister(PRE_SCALE, prescale); // Setting mode to sleep so we can change the default PWM frequency

		// Set the preferred mode.
		this->device->writeRegister(MODE1, 0xa1); // Setting mode to sleep so we can change the default PWM frequency
}


void PCA9685Driver::shutdown()
{
	/**
	 * Lock this method to prevent multiple threads from being in here simultaneously, as things could go wrong.
	 */
	std::unique_lock<std::mutex> lck(cmtx);

	/**
	 * Decrement the count indicating there is one less usage of this object.
	 */
	this->count--;

	/*
	 * Now determine if the count is 0.  If it is, no one is using this instance so it can be removed and deleted.
	 */
	if (this->count == 0)
	{
		/**
		 * Remove it from the map.
		 */
		deviceMaps.erase(address);

		/**
		 * Delete the instance
		 */
		delete this;
	}
}
