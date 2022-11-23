/**
 * @file ImageTransmitter.cpp
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

#include "ImageTransmitter.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include "time_util.h"
#include <string.h>

/**
 * This will instantiate a new instance of this class. It will copy the machine name into a heap allocated string and update the port.
 * @param machineName This is the name of the machine that the image is to be streamed to.
 * @param port This is the udp port number that the machine is to connect to.
 */
ImageTransmitter::ImageTransmitter(char *machineName, int port) {
    destinationMachineName = machineName;
    myPort = port;

}

/**
 * This is the destructor. It will free all allocated memory.
 */
ImageTransmitter::~ImageTransmitter() {
// TODO
    delete destinationMachineName;

}  

/**
 * This method will stream via udp the image to the remote device.
 * @param image This is the image that is to be sent.
 * @return The return will be 0 if successful or -1 if there is a failure.
 */
int ImageTransmitter::streamImage(Mat *image) {
\
	if ((image != NULL) && (destinationMachineName != NULL)) {

        imageCount++;


        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


		if (sockfd < 0) {
			perror("ERROR opening socket");
			return -1;
		}


        struct hostent *server = gethostbyname(destinationMachineName);

		if (server == NULL) {
			fprintf(stderr, "ERROR, no such host\n");
			return -1;;
		}


        struct sockaddr_in serv_addr;
	    int server_length = sizeof(serv_addr);
	    bzero((char *) &serv_addr, server_length);
	    serv_addr.sin_family = AF_INET;
	    memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);

	    serv_addr.sin_port = htons(myPort);


		uint32_t rows = image->rows;
		uint32_t cols = image->cols;
		uint32_t channels = image->channels();
		int msg_size = channels * cols + 28;
		char* msg_buffer = (char*)malloc(msg_size);


		uint32_t currentTimestamp = current_timestamp();
		uint32_t index = 0;

		while (index < rows) {

			((int *) msg_buffer)[0] = htonl(channels);
			((int *) msg_buffer)[1] = htonl(currentTimestamp);
			((int *) msg_buffer)[2] = htonl(current_timestamp());
			((int *) msg_buffer)[3] = htonl(imageCount);
			((int *) msg_buffer)[4] = htonl(rows);
			((int *) msg_buffer)[5] = htonl(cols);
			((int *) msg_buffer)[6] = htonl(index);

			Mat row = image->row(index++);

			memcpy(&msg_buffer[28], &row.data[0], cols * channels);

			int send = sendto(sockfd, msg_buffer, msg_size, 0, (struct sockaddr *) &serv_addr, server_length);
			if (send < 0) {
				printf("Error when sending\n");
				return -1;
			}

		}

		free(msg_buffer);

		close(sockfd);

	}
	return 0;
}
