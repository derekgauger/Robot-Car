/*
 * TaskRates.h
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
 * This file defines the task rates for all periodic tasks within the system.
 */

#ifndef TASKRATES_H_
#define TASKRATES_H_

#include "Cameracfg.h"

/**
 * This is the task rate for the horn controller.
 */
#define HORN_TASK_PERIOD (125000)
#define HORN_TASK_PRIORITY (10)

/**
 * This macro defines the task rate for the motor controllers.  All 4 motors on the robot run at the same rate.
 */
#define MOTOR_CTRL_TASK_PERIOD (20000)
#define MOTOR_CTRL_TASK_PRIORITY (10)

/**
 * This macro defines the task rate for the collision sensor.
 */
#define COLLISION_SENSOR_TASK_PERIOD (100000)
#define COLLISION_SENSOR_TASK_PRIORITY (10)

/**
 * This defined the priority for the line tracker.
 */
#define LINE_TRACKER_SENSOR_TASK_PERIOD (40000)
#define LINE_TRACKER_SENSOR_TASK_PRIORITY (10)

/**
 * This variable defines the task rate for the distance sensor.  It senses the distance to objects.
 */
#define DISTANCE_SENSOR_TASK_PERIOD (75000)
#define DISTANCE_SENSOR_TASK_PRIORITY (54) 

/**
 * These variables control the Image stream.
 */
#define IMAGE_STREAM_TASK_PERIOD ((1000000/fps))
#define IMAGE_STREAM_TASK_PRIORITY (10)

/**
 * These variables set up the camera task rate.
 */
#define CAMERA_TASK_PERIOD (1000000/FPS)
#define CAMERA_TASK_PRIORITY (10)

#define ROBOT_STATUS_MANAGER_TASK_PERIOD (500000)
#define ROBOT_STATUS_MANAGER_TASK_PRIORITY (10)

/**
 * Non periodic tasks and their priorities.
 */
#define ROBOT_CONTROLLER_PRIORITY (24)
#define NETWORK_RECEPTION_TASK_PRIORITY (23)
#define NETWORK_TRANSMIT_TASK_PRIORITY (11)



#endif /* TASKRATES_H_ */
