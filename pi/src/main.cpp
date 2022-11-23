/**
 * @file main.cpp
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE This is a piece of a template solution to the lab.
 *
 *
 * @section DESCRIPTION
 *      This is the main method for the basic robot.  It will control the simplest operation of the robot over the network.
 */
using namespace std;
#include "ImageTransmitter.h"
#include "Camera.h"
#include "ImageCapturer.h"
#include <chrono>
#include <pthread.h>
#include <iostream>
#include "RunnableClass.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include "GPIO.h"
#include "NetworkManager.h"
#include "RobotController.h"
#include "NetworkCommands.h"
#include "RobotCfg.h"
#include "TaskRates.h"
#include "DistanceSensor.h"
#include "TaskRates.h"
#include "Horn.h"
#include "CollisionSensor.h"
#include "LineSensor.h"
#include "RobotStatusManager.h"
#include "CollisionSensingRobotController.h"
#include "GenericThreadInfo.h"
#include "labcfg.h"
using namespace std;

/**
 * This is the main program.  It will instantiate a network manager and robot controller as well as a Command Queue.
 * It will then block until the user enters a message on the console.
 * This will then cause it to shutdown the other classes and wait for their threads to terminate.
 */
int main(int argc, char *argv[]) {
	// These are the image sizes for the camera (c) and the transmitted image (t), both height (h) and width (w).
	int cw, ch, tw, th, fps, lpudp;

	if ( argc != 8 ) {
		printf(
				"Usage: %s ip port cameraWidth cameraHeight TransmitWidth transmitHeight <frame per second to send>",
				argv[0]);
		exit(0);
	}

	GenericThreadInfo mainThread("main", syscall(SYS_gettid));

	cout << "Main thread id is : " << mainThread.getThreadID() << "\n";

	// Convert the parameters into integers.
	int port = atoi(argv[2]);
	cw = atoi(argv[3]);
	ch = atoi(argv[4]);
	tw = atoi(argv[5]);
	th = atoi(argv[6]);
	fps = atoi(argv[7]);

	CommandQueue *myQueue[NUMBER_OF_QUEUES];
	for (int index = 0; index < NUMBER_OF_QUEUES; index++)
	{
		myQueue[index] = new CommandQueue();
	}

	/**
	 * Declare the network manager, which will receive commands from the network.
	 */
	NetworkManager nm(9090, myQueue, "NetworkManager");
	NetworkTransmissionManager ntm(&nm, "NW Trans Manager");

	/**
	 * Declare instances of the Distance Sensor and CollisionSensor classes.
	 */
	se3910RPiHCSR04::DistanceSensor ds(DS_TRIG, DS_ECHO, "Distance Sensor", DISTANCE_SENSOR_TASK_PERIOD);
	CollisionSensor cs(myQueue[0], myQueue[1], &ds, "CollisionSensor", COLLISION_SENSOR_TASK_PERIOD);
	cs.setMinimumAcceptableDistance(250);


#if LAB_IMPLEMENATION_STEP >= 12
	CollisionSensingRobotController mc(myQueue[0], myQueue[1], &cs, "CSRobotController");
#else


	/**
	 * Declare an instance of the robot controller, which will control the operation of the robot.
	 */
	RobotController mc(myQueue[0], myQueue[1], "RobotController");
#endif

	/**
	 * Declare an instance of a Horn Controller.
	 */
	Horn h(myQueue[1], BUZZER_GPIO_PIN, "Horn Controller", HORN_TASK_PERIOD);


#if LAB_IMPLEMENATION_STEP >= 10
	RobotStatusManager rsm(&ntm, &ds, "Robot Status Manager", ROBOT_STATUS_MANAGER_TASK_PERIOD);
#endif

#if LAB_IMPLEMENATION_STEP >= 11
	// Instantiate a camera.
	Camera myCamera(cw, ch, "Camera", CAMERA_TASK_PERIOD);

	// Figure out the port to use.
	ImageTransmitter it(argv[1], port);
	ImageCapturer is(&myCamera, &it, tw, th, "Image Stream", (IMAGE_STREAM_TASK_PERIOD));
#endif

	/**
	 * Declare an instance of the LineSensor.
	 */
	LineSensor ls(myQueue[2], myQueue[0], LEFT_LINE_SENSOR_GPIO_PIN,
			CENTER_LINE_SENSOR_GPIO_PIN, RIGHT_LINE_SENSOR_GPIO_PIN,
			"Stop Line Sensor Task", LINE_TRACKER_SENSOR_TASK_PERIOD);

	// Start each of the two threads up.
	nm.start(NETWORK_RECEPTION_TASK_PRIORITY);
	ntm.start(NETWORK_TRANSMIT_TASK_PRIORITY);
#if LAB_IMPLEMENATION_STEP >= 10
	rsm.start(ROBOT_STATUS_MANAGER_TASK_PRIORITY);
#endif

	mc.start(MOTOR_CTRL_TASK_PRIORITY-1);
	h.start(HORN_TASK_PRIORITY);
	ds.start(DISTANCE_SENSOR_TASK_PRIORITY);
	cs.start(COLLISION_SENSOR_TASK_PRIORITY);
	ls.start(LINE_TRACKER_SENSOR_TASK_PRIORITY);

#if LAB_IMPLEMENATION_STEP >= 11
	myCamera.start(CAMERA_TASK_PRIORITY);
	is.start(IMAGE_STREAM_TASK_PRIORITY);
#endif
	string msg;
	cin >> msg;

	while (msg.compare("QUIT") != 0) {
		if (msg.compare("P") == 0) {
			RunnableClass::printThreads();
		} else if (msg.compare("R") == 0) {
			RunnableClass::resetAllThreadInformation();
		}
		else if (msg.compare("M")==0)
		{
			// Mute the horn...
			myQueue[1]->enqueue(HORN_MUTE_COMMAND);
		}
		cin >> msg;
	}
#if LAB_IMPLEMENATION_STEP >= 11
	is.stop();
	myCamera.stop();
#endif
	ls.stop();
	cs.stop();
	ds.stop();
	h.stop();
	mc.stop();
#if LAB_IMPLEMENATION_STEP >=10
	rsm.stop();
#endif
	ntm.stop();
	nm.stop();

	// Wait for the threads to die.
#if LAB_IMPLEMENATION_STEP >= 11
	is.waitForShutdown();
	myCamera.waitForShutdown();
#endif
	ls.waitForShutdown();
	cs.waitForShutdown();
	ds.waitForShutdown();
	h.waitForShutdown();
	mc.waitForShutdown();
#if LAB_IMPLEMENATION_STEP >= 10
	rsm.waitForShutdown();
#endif
	ntm.waitForShutdown();
	nm.waitForShutdown();

	for (int index = 0; index < NUMBER_OF_QUEUES; index++)
	{
		delete myQueue[index];
	}
}

