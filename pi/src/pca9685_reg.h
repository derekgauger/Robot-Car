/*
 * File:   pca9685_reg.h
 * Author: DevXP
 *
 * GNU GENERAL PUBLIC LICENSE
 * Created on April 2, 2012, 8:28 AM
 */

#ifndef pca9685_H
#define pca9685_H
/* General registers */
#define PCA9685 0x80                    // I2C address for PCA9865 with all inputs at zero
#define   Reset   0x01         // Reset the device
#define MODE1   0x00          // 0x00 location for Mode1 register address
#define MODE2   0x01          // 0x01 location for Mode2 reigster address
#define LED0    0x06          // location for start of LED0 registers
#define LEDCOUNT 15          // number of LEDS to light 15 max
#define PRE_SCALE (0xFE)

/* Devices */
#define LEDDRV1    0xb80                // 1st PCA9685
#define LEDDRV2    0xb82                // 2nd PCA9685
#define LEDDRV3    0xd84                // 3rd PCA9685
#define LEDDRV4    0xd86                // 4th PCA9685
#define LEDDRV5    0xc88                // 5th PCA9685
#define LEDDRV6    0xc8a                // 6th PCA9685
#define LEDDRV7    0xc8c                // 7th PCA9685
#define LEDDRV8    0xc8e                // 8th PCA9685
#define LEDDRV9    0xc90                // 9th PCA9685
/* MODE1 bits */
#define PCA9685_RESTART 0x80
#define PCA9685_EXTCLK  0x40
#define PCA9685_AI      0x20
#define PCA9685_SLEEP   0x10
#define PCA9685_SUB1    0x08
#define PCA9685_SUB2    0x04
#define PCA9685_SUB3    0x02
#define PCA9685_ALLCALL 0x01

/* MODE2 bits */
#define PCA9685_INVRT   0x10
#define PCA9685_OCH     0x08
#define PCA9685_OUTDRV  0x04
#define PCA9685_OUTNE1  0x02
#define PCA9685_OUTNE0  0x01

/* LEDX_ON_H bits */
#define PCA9685_LED_ON 0x10

/* LEDX_OFF_H bits */
#define PCA9685_LED_OFF 0x10
/* PWM data variables, 16bit vide, 12bit used*/
long PWMData[16] = {
//MSB        LSB
		0b010000000000,// Channel 0
		0b101000000000,    // Channel 1
		0b000011000000,    // Channel 2
		0b000001100000,    // Channel 3
		0b000001011010,    // Channel 4
		0b000000000000,    // Channel 5
		0b000000000000,    // Channel 6
		0b000000000000,    // Channel 7
		0b000000000000,    // Channel 8
		0b000000000000,    // Channel 9
		0b000000000000,    // Channel 10
		0b000000000000,    // Channel 11
		0b000000000000,    // Channel 12
		0b000000000000,    // Channel 13
		0b000000000000,    // Channel 14
		0b000000000000     // Channel 15
		};
#endif
