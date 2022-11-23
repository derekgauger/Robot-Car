/**
 * @file ImageTransmitter.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
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
 *      This class will transmit an image to a remote device.  The image will be transmitted as a set of UDP datagrams.
 */

#ifndef IMAGETRANSMITTER_H_
#define IMAGETRANSMITTER_H_

#include <opencv2/opencv.hpp>

using namespace cv;

class ImageTransmitter {
private:
	/**
	 * This is the default port that is to be used for the UDP transmission.
	 */
	int myPort = 6000;
	/**
	 * This is the socket fd that is to be used.
	 */
	int sockfd = 0;
	/**
	 * This is a c style string representing the destination machine's name.
	 */
	char* destinationMachineName = NULL;
	/**
	 * This is a count of the images that are streamed. The first imagfe streamed will be 0 and it will increment each time a new image is streamed.
	 */
	int imageCount = 0;

public:
	/**
	 * This will instantiate a new instance of this class. It will copy the machine name into a heap allocated string and update the port.
	 * @param machineName This is the name of the machine that the image is to be streamed to.
	 * @param port This is the udp port number that the machine is to connect to.
	 */
	ImageTransmitter(char *machineName, int	port);

	/**
	 * This is the destructor. It will free all allocated memory.
	 */
	virtual ~ImageTransmitter();

	/**
	 * This method will stream via udp the image to the remote device.
	 * @param image This is the image that is to be sent.
	 * @return The return will be 0 if successful or -1 if there is a failure.
	 */
	int streamImage(Mat* image);

};

#endif /* IMAGETRANSMITTER_H_ */
