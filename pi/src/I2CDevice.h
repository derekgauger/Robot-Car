/**
 * @file I2C.h
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
#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
#include <map>

namespace se3910RPi {

/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any type of I2C device and read or write to its registers.
 */

class I2CDevice {
private:
	/**
	 * This map will provide a mapping between device ID's and the instances which communicate with them.
	 */
	static std::map<uint16_t, I2CDevice*> deviceMaps;

	/**
	 * This is a count of how many devices are using this particular I2C Device.
	 */
	uint32_t count;

	/**
	 * This is the device ID that is to be on the IIC Device.
	 */
	uint16_t device;
	/**
	 * This is the given file id for the open file.
	 */
	int file;

	/**
	 * Constructor for the I2CDevice class. It requires device number. The constructor
	 * opens a file handle to the I2C device, which is destroyed when the destructor is called
	 * @param device The device ID on the bus.
	 */
	I2CDevice(uint16_t device);

	/**
	 * Open a connection to an I2C device
	 * @return -1 on failure to open to the bus or device, 0 on success.
	 */
	virtual int init();
public:
	/**
	 * This method will obtain an instance of an I2C device.  If the device ID already has been created, it will return
	 * the existing instance after incrementing the count.  If the device does not exist, a new instance will be instantiated.
	 * @param This is the I2C device ID that is to be accessed.
	 * @return A pointer to the instance will be returned.
	 */
	static I2CDevice* obtainDeviceInstance(uint16_t device);

	/**
	 * Write a single value to the I2C device. Some devices accept data this way without needing to access any internal registers.
	 * @param value the value to write to the device
	 * @return -1 on failure to write, 0 on success.
	 */
	virtual int write(uint8_t value);

	/**
	 * Read a single register value from the address on the device.
	 * @param registerAddress the address to read from
	 * @return the byte value at the register address.
	 */
	virtual uint8_t readRegister(uint16_t registerAddress);

	/**
	 * Write a single byte value to a single register.
	 * @param registerAddress The register address
	 * @param value The value to be written to the register
	 * @return -1 on failure to write, 0 on success.
	 */
	virtual int writeRegister(uint16_t registerAddress, uint8_t value);

	virtual void shutdown();

	virtual ~I2CDevice();
};

}

#endif /* I2C_H_ */
