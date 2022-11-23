/**
 * @file I2C.cpp
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
 * This file contains a set of I2C wrappers.  This class allows a user
 * of the Raspberry PI to readily interface with the wiringPi libraries in an
 * object oriented fashion in C++.
 */

#include"I2CDevice.h"
#include<iostream>
#include<sstream>
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>
#include<sys/ioctl.h>

#include <wiringPiI2C.h>

using namespace std;

namespace se3910RPi {

/**
 * This map will provide a mapping between device ID's and the instances which communicate with them.
 */
std::map<uint16_t, I2CDevice*> I2CDevice::deviceMaps;

/**
 * This method will obtain an instance of an I2C device.  If the device ID already has been created, it will return
 * the existing instance after incrementing the count.  If the device does not exist, a new instance will be instantiated.
 * @param This is the I2C device ID that is to be accessed.
 * @return A pointer to the instance will be returned.
 */
I2CDevice* I2CDevice::obtainDeviceInstance(uint16_t deviceID)
{
	/**
	 * Determine if the device already has been instantiated.
	 */
	if (deviceMaps.find(deviceID) == deviceMaps.end())
	{
		/**
		 * There is no instance.  Make one and insert it.
		 */
		deviceMaps.insert(std::make_pair(deviceID, new I2CDevice(deviceID)));
		deviceMaps[deviceID]->init();
	}
	/**
	 * Increment the instance count for this device.
	 */
	deviceMaps[deviceID]->count++;
	return deviceMaps[deviceID];
}

/**
 * Constructor for the I2CDevice class. It requires device number. The constructor
 * opens a file handle to the I2C device, which is destroyed when the destructor is called
 * @param device The device ID on the bus.
 */
I2CDevice::I2CDevice(uint16_t device) {
	this->file=-1;
	this->device = device;
}

/**
 * Open a connection to an I2C device
 * @return -1 on failure to open to the bus or device, 0 on success.
 */
int I2CDevice::init(){
	this->file = wiringPiI2CSetup (this->device) ;

	if (this->file > 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/**
 * Write a single value to the I2C device. Some devices accept data this way without needing to access any internal registers.
 * @param value the value to write to the device
 * @return -1 on failure to write, 0 on success.
 */
int I2CDevice::write(uint8_t value){
	if (this->file < 0)
	{
		return -1;
	}
	else
	{
	    return wiringPiI2CWrite (this->file, value) ;
	}
}

/**
 * Write a single byte value to a single register.
 * @param registerAddress The register address
 * @param value The value to be written to the register
 * @return -1 on failure to write, 0 on success.
 */
int I2CDevice::writeRegister(uint16_t registerAddress, uint8_t value)
{
	if (this->file < 0)
	{
		return -1;
	}
	else
	{
	    return wiringPiI2CWriteReg8 (this->file, registerAddress, value) ;
	}
}

/**
 * Read a single register value from the address on the device.
 * @param registerAddress the address to read from
 * @return the byte value at the register address.
 */
uint8_t I2CDevice::readRegister(uint16_t registerAddress){
	return wiringPiI2CReadReg8 (this->file, registerAddress) ;
}


/**
 * Close the file handles and sets a temporary state to -1.
 */
void I2CDevice::shutdown(){
	this->file = -1;
	close(this->file);
}

/**
 * Closes the file on destruction, provided that it has not already been closed.
 */
I2CDevice::~I2CDevice() {
	if(file!=-1) this->shutdown();
}

} /*  */
