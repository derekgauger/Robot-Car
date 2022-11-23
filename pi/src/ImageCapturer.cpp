#include "ImageCapturer.h"
#include <chrono>

using namespace std::chrono;
using namespace std;

/**
 * Construct a new instance of the image capturer. It will instantiate an instance of the Size class.
 * @param referencedCamera This is the camera that is referenced.
 * @param trans This is the image transmitter that is to send the given image across the network.
 * @param width This is the width of the image that is to be sent in pixels.
 * @param height This is the height of the image that is to be sent in pixels.
 * @param threadName This is the name of the thread that is to execute.
 * @param period This is the period for the task, given in microseconds.
 */
ImageCapturer::ImageCapturer(Camera *referencedCamera, ImageTransmitter *trans,
		int width, int height, std::string threadName, uint32_t period) :
		PeriodicTask(threadName, period) {
	myCamera = referencedCamera;
	myTrans = trans;
	imageWidth = width;
	imageHeight = height;
	size = new Size(width, height);
}

/**
 * This is the destructor.
 */
ImageCapturer::~ImageCapturer() {
	delete size;
}

/**
 * This is the virtual task  method. It will execute the given code that is to be executed by this class. It will execute once each task period. The algorithm is as follows:
 */
void ImageCapturer::taskMethod() {
	static milliseconds totalGrab(0);
	static milliseconds totalResize(0);
	static milliseconds totalTransmit(0);
	static long framesTransmitted = 0;



	/**
	 * 1.0 Obtain the time since the epoch from the system clock in ms.
	 */
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	/**
	 *2.0 Take the picture from the camera.
	 */
	Mat *image = myCamera->takePicture();

	/**
	 * 3.0 If the image is not empty,
	 */
	if (!image->empty()) {
		/**
		 * 3.1 Obtain the time since the epoch from the system clock in ms.
		 */
		milliseconds start2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		/**
		 * 3.2 Resize the image according to the desired size, if a resize needs to occur.
		 */
		Mat dst;
		resize(*image, dst, *size);

		/**
		 * Convert the image to greyscale.
		 */
		Mat gsdst;
		cvtColor(dst, gsdst, COLOR_BGR2GRAY);

		/**
		 * 3.4 Obtain the time since the epoch from the system clock in ms.
		 */
		milliseconds start3 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		/**
		 * 3.5 Stream the image to the remote device.
		 */
		myTrans->streamImage(&gsdst);

		/**
		 * 3.6 Obtain the time since the epoch from the system clock in ms.
		 */
		milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		milliseconds delta = start2 - start;

		/**
		 * 3.7 Print out to the console in ms the amount of time it took to grab the picture, resize the picture, and transmit the picture in ms.
		 */
		cout << "Count: " << count++ << "\t";
		cout << "Grab picture:\t" << (delta.count()) << "\t";
		totalGrab = totalGrab + delta;

		delta = start3 - start2;
		cout << "Resize:\t" << (delta.count()) << "\t";
		totalResize += delta;

		delta = end - start3;
		totalTransmit+= delta;

		cout << "Transmit:\t" << (delta.count()) << "\t";
		milliseconds delayTime = std::chrono::milliseconds(getTaskPeriod() / 1000) - (end - start);
		if (delayTime.count() < 0) {
			xmitTimeDeadlineMissCount++;
			cout << "Delaying:\t" << delayTime.count()
			 << " Missed Deadline Count: " << xmitTimeDeadlineMissCount
			 << "\n" << std::flush;
		} else {
			cout << "Delaying:\t" << delayTime.count() << "\t" << std::flush;
		}
		framesTransmitted++;


		/**
		 * 3.8 Update the averages for each item.
		 */
		 cout << "Average Grab:\t" << totalGrab.count() / framesTransmitted << "\t";
		 cout << "Average Resize:\t" << totalResize.count() / framesTransmitted << "\t";
		 cout << "Average Transmit:\t" << totalTransmit.count() / framesTransmitted << "\r";
		 cout << flush;

	}
	delete image;
}

