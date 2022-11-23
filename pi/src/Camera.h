/*
 * Camera.h
 * This class will use the OpenCV Video capture feature to capture images from the camera on the Raspberry Pi.
 * It is a periodic task.
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "PeriodicTask.h"
#include <opencv2/opencv.hpp>
#include <mutex>

using namespace std;
using namespace cv;

class Camera: public PeriodicTask {
private:
	/**
	 * The video capture is an instance of the OpenCV Image capture class. It is instantiated in the constructor and used to capture images.
	 */
	VideoCapture *capture;

	/**
	 * This is the last frame that was captured by the camera. It is in an OpenCV Matrix format.
	 */
	Mat *lastFrame;

	/**
	 * This is a mutex within the camera class that prevents race conditions as the images are manipulated.
	 */
	std::mutex mtx;
public:
	/**
	 * Construct a new instance of the camera class.
	 * @param width This is the width of the natively captured images. This is the resolution the camera generates.
	 * @param height This is the height of the natively captured images. This is the resolution the camera generates.
	 * @param threadName This is the name of the thread that is to be used to run the image capture.
	 * @param period This si the period for the periodic task.
	 */
	Camera(int width, int height, std::string threadName, uint32_t period);

	/**
	 * This is the destructor for the camera. It will delete all dynamically allocated objects.
	 */
	virtual ~Camera();

	/**
	 * This is the main thread for the camera. It will do the following:
	 */
	void taskMethod();

	/**
	 * This method will return the next picture from the camera, following the algorithms described here:
	 * @return The return will be a matrix of the picture that was last grabbed from the camera.
	 */
	Mat *takePicture();
};
#endif /* CAMERA_H_ */

