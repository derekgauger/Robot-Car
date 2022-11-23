package edu.msoe.se3910.imageTransmit.core;

/**
 * Thsi class will be used to capture an image that is sent over the network.
 */
public class CapturedImage {
    /**
     * This is the time the image transmission starts.
     */
    private int startTime;
    /**
     * This is the time the image transmission finishes.
     */
    private int finishTime;
    /**
     * The height of the image in pixels.
     */
    private int height;
    /**
     * The width of the image in pixels.
     */
    private int width;
    /**
     * The unique id for the image.
     */
    private int id;
    /**
     * This is the number of bytes sent out for each pixel.
     */
    private int bytesPerPixel;

    /**
     * The array holds the raw data for the image as it is received.
     */
    private byte myArray[];
    /**
     * This indicates that reception has completed.
     */
    private boolean done;
    /**
     * This indicates if the image is valid.
     */
    private boolean valid = false;
    /**
     * This is a count of the number of lines received in the captured image.
     */
    private int linesReceived = 0;

    public byte[] getMyArray() {
        return myArray;
    }

    public int getId() {
        return id;
    }

    public boolean isDone() {
        return done;
    }

    /**
     * Invoking this method will rest the image, but will not wipe the buffer.  This will allow for retained lines to be kept, better supporting envionments where UDP Datagram losses are occuring.
     */
    public void resetImage(int id, int startTime)
    {
        linesReceived = 0;
        this.id = id;
        this.startTime=startTime;
        done = false;
    }

    public CapturedImage(int h, int w, int bytesPerPixel, int id, int startTime) {
        height = h;
        width = w;
        this.bytesPerPixel = bytesPerPixel;
        this.id = id;
        myArray = new byte[3 * height * width];
        this.startTime=startTime;
    }

    /**
     * This method will return the time from the first transmission to the final transmission in ms.
     * @return The delta of transmitting the image will be returned.
     */
    public int getTransmissionTime() {
        int retVal;
        if (startTime < finishTime) {
            retVal = finishTime - startTime;
        } else {
            retVal = (0x7FFFFFFF - startTime) + finishTime;
        }
        return retVal;
    }

    public void addLine(byte[] line, int cseg) {
        // Now we need to copy this into the main image buffer.
        System.arraycopy(line, 0, myArray, bytesPerPixel * width * cseg, bytesPerPixel * width);
        linesReceived++;
        if (linesReceived == height) {
            done = true;
        }
        if (linesReceived > height / 5)
        {
            valid = true;
        }
    }
    public int getHeight() {return height;};

    public int getLinesReceived()
    {
        return linesReceived;
    }

    public void setFinishTime(int finishTime)
    {
        this.finishTime=finishTime;
    }

    public boolean isValid() {
        return valid;
    }
}