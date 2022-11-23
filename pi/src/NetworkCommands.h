/**
 * @file NetworkCommands.h
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
 * This file defines the commands that are to be sent over the network to the device.
 */

#ifndef NETWORKCOMMANDS_H_
#define NETWORKCOMMANDS_H_

/**
 * This segment defines the destination queues and devices for the given messages.
 */
#define MOTOR_CONTROL_DESTINATION (0x00000001)

/**
 * This command indicates that the rest of the word being sent over the network is consumed by the speed setting.
 */
#define SPEEDDIRECTIONBITMAP  0x40000000

/**
 * These are definitions related to the control of the robot via the network.
 */
#define MOTORDIRECTIONBITMAP  0x20000000

/**
 * These are definitions related to the control of the robot via the network.
 */
#define STEERINGOFFSETBITMAP  0x10000000

/**
 * This indicates the value that is or'ed with the MOTORDIRECTIONBITMAP to indicate that the direction of the robot is to be forward.
 */
#define  FORWARD  0x00000001
/**
 * This indicates the value that is or'ed with the MOTORDIRECTIONBITMAP to indicate that the direction of the robot is to be backward.
 */
#define  BACKWARD  0x00000002
/**
 * This indicates the value that is or'ed with the MOTORDIRECTIONBITMAP to indicate that the direction of the robot is to be left.
 */
#define  LEFT  0x00000004
/**
 * This indicates the value that is or'ed with the MOTORDIRECTIONBITMAP to indicate that the direction of the robot is to be right.
 */
#define  RIGHT  0x00000008
/**
 * This indicates the value that is or'ed with the MOTORDIRECTIONBITMAP to indicate that the robot is to stop.
 */
#define  STOP  0x00000010

/**
 * This definition is a combination of all motor directions, and can be used as a generic mask when commands are sent.
 */
#define MOTORDIRECTIONS (FORWARD | BACKWARD | LEFT | RIGHT| STOP)



/**
 * This indicates that the horn sound is to be muted.
 */
#define HORN_MUTE_COMMAND  (0x40000000)

/**
 * This command indicates that the horn is to sound continuously.
 */
#define HORN_SOUND_COMMAND (0x20000000)

/**
 *  This indicates that the command received is a command to pulse the horn.
 *  The lowest 12 bits indicate the period for the horn sound in ms.
 *  The next 12 bits indicate the time that the horn is to sound in ms.
 *  The robot may approximate these periods based upon the periodic task rate selected.
 */
#define HORN_PULSE_COMMAND (0x10000000)


#define START_LINE_SENSING (0x40000000)
#define STOP_LINE_SENSING  (0x20000000)
#define ENABLE_LINE_FOLLOWING  (0x04000000)
#define DISABLE_LINE_FOLLOWING (0x02000000)

/**
 * This command here is related to reporting back of distance measurements.
 */
#define DISTANCE_MEASUREMENT_REPORT (0x40000000)
#define DISTANCE_MEASUREMENT_REPORT_CURRENTREADINGBITMAP   (0x08000000)
#define DISTANCE_MEASUREMENT_REPORT_AVEREADINGBITMAP       (0x04000000)
#define DISTANCE_MEASUREMENT_REPORT_MAXREADINGBITMAP       (0x02000000)
#define DISTANCE_MEASUREMENT_REPORT_MINREADINGBITMAP       (0x01000000)

#endif /* NETWORKCOMMANDS_H_ */
