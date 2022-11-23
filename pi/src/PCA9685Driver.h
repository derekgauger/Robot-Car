/*
 * PCA9685Driver.h
 *
 *  Created on: Jul 2, 2018
 *      Author: se3910
 */

#ifndef PCA9685DRIVER_H_
#define PCA9685DRIVER_H_

#include "I2CDevice.h"
#include <stdint.h>

class PCA9685Driver {
private:
	static std::map<uint16_t, PCA9685Driver*> deviceMaps;
	se3910RPi::I2CDevice *device;
	unsigned int address;

	/**
	 * This is a count of how many devices are using this particular I2C Device.
	 */
	uint32_t count;

	PCA9685Driver(uint16_t deviceAddress);

	virtual ~PCA9685Driver();

public:
	/**
	 * This method will obtain an instance of an I2C device.  If the device ID already has been created, it will return
	 * the existing instance after incrementing the count.  If the device does not exist, a new instance will be instantiated.
	 */
	static PCA9685Driver* obtainPCA9685Instance(uint16_t device);

	void setPwm(unsigned int channel, uint16_t onTime, uint16_t offTime);
	/**
	 * This method will set the width of the given pulse.
	 * @param percent This is the percent of the duty cycle, multipled by 10.  A value of 10 represents 10%.  A value of 1 represents .1%.
	 */
	void setDutyCycle(unsigned int channel, uint16_t percent);

	/**
	 * This method will set the frequency for the PWM module driven by this device.
	 * @freq This is the frequency, given in hertz.
	 */
	void setFrequency(uint32_t freq);


	void init();
	void shutdown();



};

#endif /* PCA9685DRIVER_H_ */
