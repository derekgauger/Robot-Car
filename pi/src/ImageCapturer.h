/*
 * ImageCapturer.h
 * This class is responsible for capturing an image from the camera and getting it ready to be transmitted to another device.
 */

#ifndef IMAGECAPTURER_H_
#define IMAGECAPTURER_H_

#include "PeriodicTask.h"
#include "Camera.h"
#include "ImageTransmitter.h"

class ImageCapturer: public PeriodicTask {
private:
	/**
	 * This is a pointer to the camera that is gong to be used to capture the images.
	 */
	Camera* myCamera;

	/**
	 * This is a pointer to the image transmitter that will transmit the image to the other device.
	 */
	ImageTransmitter *myTrans;

	/**
	 * This is the width of the image that is to be transmitted in pixels. It may or may not be the same as the width captured by the camera.
	 */
	int imageWidth;

	/*
	 * This is the image height that is to be transmitted.
	 */
	int imageHeight;

	/**
	 * This is the size of the image that is to be transmitted. It is an openCV Size type.
	 */
	Size *size;

	/**
	 * This is the count of the images taken.  It will icnrement each time a new photo is captured.
	 */
	int count = 0;

	/**
	 * This variable will keep track of how many times the image has failed to transmit in an appropriate amount of time (i.e. we have not ttransmitted fast enough.)
	 */
	int xmitTimeDeadlineMissCount=0;
public:

	/**
	 * Construct a new instance of the image capturer. It will instantiate an instance of the Size class.
	 * @param referencedCamera This is the camera that is referenced.
	 * @param trans This is the image transmitter that is to send the given image across the network.
	 * @param width This is the width of the image that is to be sent in pixels.
	 * @param height This is the height of the image that is to be sent in pixels.
	 * @param threadName This is the name of the thread that is to execute.
	 * @param period This is the period for the task, given in microseconds.
	 */
	ImageCapturer(Camera *referencedCamera, ImageTransmitter *trans, int width, int height, std::string threadName, uint32_t period);

	/**
	 * This is the destructor for the class.
	 */
	virtual ~ImageCapturer();

	/**
	 * This is the taskMethod that will run.
	 */
	virtual void taskMethod();
};
#endif /* IMAGECAPTURER_H_ */
