/**
 * @file RobotCfg.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
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
 *      This file allows us to easily configure the robot for different IO setups. 
 */

#ifndef ROBOTCFG_H_
#define ROBOTCFG_H_

#define FREENOVE

/**
 * The following definitions are for the ALPHABOT.
 */
#ifdef ALPHABOT
#define AIN1 (12)
#define AIN2 (13)
#define ENA (6)
#define ENB (26)
#define IN3 (20)
#define IN4 (21)
#define DL (16)
#define DR (19)

#endif

/**
 * The following definitions are for the ALPHABOT2.  Basically, it defines the GPIO pin for various items.
 */
#ifdef ALPHABOT2
#define AIN1 (12)
#define AIN2 (13)
#define ENA (6)
#define ENB (26)
#define BIN1 (20)
#define BIN2 (21)
#define DL (16)
#define DR (19)
#define BUZZER (4)
#define IOCLK (25)
#define ADDR (24)
#define DOUT (23)
#define CHIPSEL (5)

#endif

#ifdef FREENOVE
/**
 * This macro defines the address for the motor PWM driver on the I2C Bus.
 */
#define PCADEVICE_ADDR (0x40)

/**
 * These macros define the motor F and R Channels.
 */
#define LFMFCHANNEL (0x00)
#define LFMRCHANNEL (0x01)

#define LRMFCHANNEL (0x03)
#define LRMRCHANNEL (0x02)

#define RFMFCHANNEL (0x04)
#define RFMRCHANNEL  (0x05)

#define RRMFCHANNEL (0x06)
#define RRMRCHANNEL (0x07)









#define BUZZER_GPIO_PIN (17)
#define DS_ECHO (22)
#define DS_TRIG (27)

#define LEFT_LINE_SENSOR_GPIO_PIN (14)
#define CENTER_LINE_SENSOR_GPIO_PIN (15)
#define RIGHT_LINE_SENSOR_GPIO_PIN (23)



#endif

#endif /* ROBOTCFG_H_ */
