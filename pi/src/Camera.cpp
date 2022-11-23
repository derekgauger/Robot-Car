#include "Camera.h"
#include "time_util.h"
#include <chrono>
#include <sys/time.h>
#include "Cameracfg.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp> // TODO do we need this
#include <opencv2/core/mat.hpp> // TODO this too?

using namespace cv;
using namespace std;

/**
 * Construct a new instance of the camera class.
 * @param width This is the width of the natively captured images. This is the resolution the camera generates.
 * @param height This is the height of the natively captured images. This is the resolution the camera generates.
 * @param threadName This is the name of the thread that is to be used to run the image capture.
 */
Camera::Camera(int width, int height, string threadName, uint32_t period) :
		PeriodicTask(threadName, period) {

	/**
	 * 1.0 Start by instantiating a VideoCapture object which will grab the images from the camera.
	 */
	capture = new VideoCapture(0); // TODO: look into if camera 0 is correct

	/**
	 * 2.0 Next, set the width, height, and frames per second parameters of the capture.
	 */

	capture->set(CAP_PROP_FRAME_WIDTH, width);

	capture->set(CAP_PROP_FRAME_HEIGHT, height);

	capture->set(CAP_PROP_FPS, FPS);


	/**
	 * 3.0 Instantiate a new matrix that will hold the last frame.
	 */
	lastFrame = new Mat();


	/**
	 * 4.0 Check to see that the capture is opened.  If if isn't, print out a failure method and exit the program with an error code.
	 */
	if (!capture->isOpened()) {
		cout << "Failed to connect to the camera." << endl;
		exit(-1);
	}
}

/**
 * This is the destructor for the camera. It will delete all dynamically allocated objects.
 */
Camera::~Camera() {
	/**
	 * 1.0 Release the camera.
	 */
	delete capture;


	/**
	 * 2.0 Delete all allocated objects.
	 */
	delete lastFrame;

// TODO mutex?

}

/**
 * This is the task method for the camera. It will do the capture images from the camera hardware, keeping the image that is available current.
 */
void Camera::taskMethod() {
	/**
	 * 1.0 Allocate a pointer to a matrix that will be the new last frame.
	 * We are doing this so that we can truly minimize the length of the critical section where we have the mutex locked.
	 */
	Mat *newLastFrame = new Mat();

	/**
	 * 2.0 Read the next frame in, placing it in the newly allocated Mat.
	 */
	capture->grab();
	capture->retrieve(*newLastFrame);
	/**
	 * 3.0 Lock the mutex that protects the last frame.
	 */
	mtx.lock();

	/**
	 * 4.0 Allocate a temporary pointer which points to a Mat and initialize it to point at the matrix pointed to by lastFrame.
	 */
	Mat *temp = lastFrame;
	/**
	 * 5.0 Set the lastFrame pointer to point at the object allocated above in step 1, holding the new frame in it.
	 */
	lastFrame = newLastFrame;
	/**
	 * 6.0 Unlock the mutex.
	 */
	mtx.unlock();
	/**
	 * 7.0 Free the Mat that is pointed to by temp now.
	 */
	delete temp;
}

/**
 * This method will return the next picture from the camera, following the algorithms described here:
 * @return The return will be a matrix of the picture that was last grabbed from the camera.  If the last frame is empty, the return will be an empty matrix.
 */
Mat *Camera::takePicture() {
	/**
	 * 1.0 Create a pointer to an empty matrix object and instantiate a new Mat.
	 */
	Mat *returnMat = new Mat();

	/**
	 * 2.0 If the last frame is not empty.
	 */
	if (!lastFrame->empty()) {
		/**
		 * 2.1 Lock the mutex protecting the last frame.
		 */
		mtx.lock();

		/**
		 * 2.2 Copy the last frame to the return value.
		 */
		lastFrame->copyTo(*returnMat);

		/**
		 * 2.3 Unlock the mutex.
		 */
		mtx.unlock();

	}
	/**
	 * 3.0 Return the pointer to the matrix object.
	 */
	return returnMat;

}

