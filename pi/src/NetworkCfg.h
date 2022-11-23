/**
 * @file NetworkCfg.h
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
 * This file defines the configuration for the network device.
 */
#ifndef NETWORKCFG_H_
#define NETWORKCFG_H_

/**
 * Define the number of queues present in the system.
 * Queue 1 is for the Robot Controller.
 * Queue 2 is for the Horn COntroller.
 * Queue 3 is for the Line Sensor.
 */
#define NUMBER_OF_QUEUES (3)


#endif /* NETWORKCFG_H_ */
