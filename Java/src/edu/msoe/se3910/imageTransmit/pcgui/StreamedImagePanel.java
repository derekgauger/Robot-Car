package edu.msoe.se3910.imageTransmit.pcgui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.HeadlessException;
import java.awt.Image;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.ComponentSampleModel;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.PixelGrabber;
import java.awt.image.Raster;
import java.awt.image.SampleModel;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import edu.msoe.se3910.imageTransmit.core.CapturedImage;
import edu.msoe.se3910.imageTransmit.core.ConnectionObserver;
import edu.msoe.se3910.imageTransmit.core.LongListWithStatistics;

/**
 * This class will receive an image that is streamed in RGB format from a remote
 * location over a socket and display it on the panel. The protocol involves
 * first sending as an integer with height and width of the item. Then the data
 * will be streamed in RGB format.
 *
 * @author wws
 */
public class StreamedImagePanel extends JPanel implements Runnable {
    private BufferedImage image;
    /**
     *
     */
    private static final long serialVersionUID = 1L;
    /**
     * This variable will be true so long as the thread is to continue executing.
     */
    private boolean keepGoing = true;

    /**
     * This is the port number the socket should listen for connections on.
     */
    private int portNumber;

    /**
     * This is the JLabel that will hold the image.
     */
    private JLabel myLabel = null;

    /**
     * This class holds data related to the performance of images and how much
     * variance there is in arrival times.
     */
    private LongListWithStatistics timingData = new LongListWithStatistics(100);

    /**
     * This is the print stream into which data is to be printed. If null, no
     * performance data will be logged.
     */
    private PrintStream logStream = null;

    /**
     * This panel will be used to help keep the diagnostic controls current.
     */
    private DiagnosticControls diagPanel;

    private NetworkTrafficStatusPanel netstatpanel;


    /**
     * This is the socket from which the data is to be read.
     */
    DatagramSocket clientSocket = null;

    private int imageWidth;
    private int imageHeight;
    private int bytesPerPixel = 3;

    /**
     * This is a timestamp of the time when the last item was received on this
     */
    private long lastReceptionTimestamp = Long.MIN_VALUE;
    private int receivedPhotoCount = 0;

    ImageIcon myIcon;

    public void changeSize(int newWidth, int newHeight) {
        if (newWidth >= 0 && newHeight >= 0) {
            try {
                super.setPreferredSize(new Dimension(newWidth, newHeight));
                image = new BufferedImage(newWidth, newHeight, BufferedImage.TYPE_3BYTE_BGR);
                myIcon.setImage(image);
                this.setWidthAndHeight(newWidth, newHeight);
                validate();
                repaint();
            } catch (Exception ex) {
                // There are casses where there may be transient wrong values sent over the network.  By catching the exception here, we allow the system to recover.  We really should not be doing this here, but this is a temporary robustness fix that will help students.
            }
        }
    }


    /**
     * This is the list of observers of this class.
     */
    private List<ConnectionObserver> observers = new ArrayList<ConnectionObserver>();

    public void setWidthAndHeight(int width, int height) {
        super.setSize(new Dimension(width, height));
        this.imageWidth = width;
        this.imageHeight = height;
    }

    public void addObserver(ConnectionObserver obs) {
        observers.add(obs);
    }

    /**
     * This method will set the log stream.
     *
     * @param logStream This is the print stream that is to be used to print
     *                  diagnostic data.
     */
    public void setLogStream(PrintStream logStream) {
        this.logStream = logStream;
    }

    /**
     * Construct a new instance of a streamed image panel.
     *
     * @param port   This is the port that is to be listened on for incoming
     *               connections.
     * @param width  This is the width of the image that is to be received.
     * @param height This is the height of the image.
     * @throws IOException This exception will be thrown if there is a problem
     *                     instantiating the socket.
     */

    public StreamedImagePanel(int port, int width, int height) throws IOException {
        super(new BorderLayout(), false);
        portNumber = port;
        this.clientSocket = new DatagramSocket(portNumber);

        image = new BufferedImage(width, height, BufferedImage.TYPE_3BYTE_BGR);
        myIcon = new ImageIcon(image);
        myLabel = new JLabel(myIcon);
        this.add(myLabel, BorderLayout.CENTER);

        diagPanel = new DiagnosticControls();
        this.add(diagPanel, BorderLayout.SOUTH);

        netstatpanel = new NetworkTrafficStatusPanel();
        this.add(netstatpanel, BorderLayout.EAST);

        this.imageWidth = width;
        this.imageHeight = height;
    }

    @Override
    public void run() {
        // So long as the thread is active, keep receiving.
        CapturedImage imageBeingReceived = new CapturedImage(this.imageHeight, this.imageWidth, bytesPerPixel, Integer.MIN_VALUE, 0);

        byte[] receptionBuffer = new byte[((this.imageWidth * bytesPerPixel + 24))];
        byte[] lineRGBDataBuffer = new byte[this.imageWidth * bytesPerPixel];

        while (keepGoing) {
            // Check that the buffer sizes are still correct. If something has changed,
            // modify the sizes again.
            if ((receptionBuffer.length != ((this.imageWidth * bytesPerPixel + 24))) || (lineRGBDataBuffer.length != this.imageWidth * bytesPerPixel)) {
                receptionBuffer = new byte[((this.imageWidth * bytesPerPixel + 24))];
                lineRGBDataBuffer = new byte[this.imageWidth * bytesPerPixel];
            }

            // Receive the next image packet.
            DatagramPacket packet = new DatagramPacket(receptionBuffer, receptionBuffer.length);

            // blocks until a packet is received
            try {
                this.clientSocket.receive(packet);

                // Now read the buffer that just was received.
                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(receptionBuffer));

                // Update the observers to indicate a new item has been received.
                for (ConnectionObserver co : observers) {
                    co.connectionEstabolished(packet.getAddress());
                }

                // Read how many bytes are in each pixel.
                bytesPerPixel = dis.readInt();

                int startTime = dis.readInt();
                int currentTime = dis.readInt();
                int cimageNumber = dis.readInt();

                int crow = dis.readInt();
                int ccol = dis.readInt();
                int cseg = dis.readInt();
                try {
                    dis.readFully(lineRGBDataBuffer, 0, bytesPerPixel * imageWidth);
                } catch (java.io.EOFException e) {
                }

                /**
                 * Determine if we are to print extra diagnostic info on each segment that is received.
                 */
                if (diagPanel.isExtraDebugInfoActive()) {
                    System.out.printf("\tFrame: %d CSeg: %d ", cimageNumber, cseg);
                    System.out.println();
                }

                if (ccol != imageWidth || crow != imageHeight) {
                    changeSize(ccol, crow);
                }

                if (cimageNumber == imageBeingReceived.getId()) {
                    // Add the data in.
                    imageBeingReceived.addLine(lineRGBDataBuffer, cseg);
                    imageBeingReceived.setFinishTime(currentTime);
                } else {
                    if (imageBeingReceived.isValid()) {
                        displayImage(imageBeingReceived);
                    }

                    // We have a new image, so show the details about it if we are supposed to do so.
                    if (diagPanel.isPrintNewHeadersActive()) {
                        System.out.println("Image Number: " + cimageNumber + "\tSize: " + crow + "x" + ccol + "\tStart Time:" + startTime);
                    }

                    // Handle a new image...
                    if (diagPanel.isRowPixelInformationToBeRetained() == false) {
                        imageBeingReceived = new CapturedImage(this.imageHeight, this.imageWidth, bytesPerPixel, cimageNumber, startTime);
                    } else {
                        imageBeingReceived.resetImage(cimageNumber, startTime);
                    }
                    imageBeingReceived.addLine(lineRGBDataBuffer, cseg);
                    receptionBuffer = new byte[this.imageWidth * bytesPerPixel + 24];
                    lineRGBDataBuffer = new byte[this.imageWidth * bytesPerPixel];
                }
                if (imageBeingReceived.isDone()) {
                    displayImage(imageBeingReceived);
                    if (diagPanel.isRowPixelInformationToBeRetained() == false) {
                        imageBeingReceived = new CapturedImage(this.imageHeight, this.imageWidth, bytesPerPixel, cimageNumber, startTime);
                    } else {
                        imageBeingReceived.resetImage(cimageNumber, startTime);
                    }
                    receptionBuffer = new byte[this.imageWidth * bytesPerPixel + 24];
                    lineRGBDataBuffer = new byte[this.imageWidth * bytesPerPixel];
                }

            } catch (IndexOutOfBoundsException ex) {
                // Normally we would never want to catch this exception. However, in the case of
                // resizing the window, an index out of bounds may get thrown.
                // This catches it. In reality, this code needs some more refactoring in here.
                // TODO Refactor so this exception does not need to be caught.
                imageBeingReceived = new CapturedImage(this.imageHeight, this.imageWidth, bytesPerPixel, 0, 0);
                receptionBuffer = new byte[((this.imageWidth * bytesPerPixel + 24))];
                lineRGBDataBuffer = new byte[this.imageWidth * bytesPerPixel];

            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }


    public void displayImage(CapturedImage receivedImage) {
        netstatpanel.setcurrentStatusValue((receivedImage.getLinesReceived() * 100) / receivedImage.getHeight());

        // Obtain the current time.
        long currentTime = java.lang.System.currentTimeMillis();

        receivedPhotoCount++;

        long receptionDelta = 0;
        // Figure out how long this took to occur.
        if (lastReceptionTimestamp != Long.MIN_VALUE) {
            receptionDelta = currentTime - lastReceptionTimestamp;
            lastReceptionTimestamp = currentTime;
        } else {
            receptionDelta = 0;
            lastReceptionTimestamp = currentTime;
        }

        // Create a new data buffer that can be used as the basis for an image.
        DataBuffer buffer = new DataBufferByte(receivedImage.getMyArray(), receivedImage.getMyArray().length);

        // The most difficult part of awt api for me to learn
        SampleModel sampleModel;

        if (bytesPerPixel == 1) {
            sampleModel = new ComponentSampleModel(DataBuffer.TYPE_BYTE, imageWidth, imageHeight, bytesPerPixel,
                    imageWidth * bytesPerPixel, new int[]{0});
            ;
        } else {
            sampleModel = new ComponentSampleModel(DataBuffer.TYPE_BYTE, imageWidth, imageHeight, bytesPerPixel,
                    imageWidth * bytesPerPixel, new int[]{2, 1, 0});
        }


        Raster raster = Raster.createRaster(sampleModel, buffer, null);

        // Create a new image and assign the photo to it.
        BufferedImage localImage;
        if (bytesPerPixel == 3) {
            localImage = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_3BYTE_BGR);
        } else if (bytesPerPixel == 1) {
            localImage = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_BYTE_GRAY);
        } else {
            localImage = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_CUSTOM);
        }
        localImage.setData(raster);
        localImage = toBufferedImage(rotateImage(localImage, 0));
        ImageIcon imageIconToUse = new ImageIcon(localImage);

        // Display the image.
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                myIcon = imageIconToUse;
                myLabel.setIcon(myIcon);
            }
        });

        // Calculate and display metrics.
        timingData.addDataItem(receptionDelta);
        double average = timingData.getAverage();
        double std = timingData.getSTD();

        if (logStream != null) {
            logStream.printf("Frame: %d \tLinesReceived: %d\tHeight: %d\tWidth: %d\tDelta: %d\tMean:%.2f\tSTD:%.2f\tXmit Delta: %d\n",
                    receivedPhotoCount, receivedImage.getLinesReceived(), imageHeight, imageWidth, receptionDelta,
                    average, std, receivedImage.getTransmissionTime());


//					"Frame: " + receivedPhotoCount + "\tLines Received: " + receivedImage.getLinesReceived() + '
//		'"\tHeight: " + imageHeight + "\tWidth: " + imageWidth
//					+ "\tDelta: " + receptionDelta + "\tMean: " + average + "\tSTD: " + std + "\tLatency: "
//					+ receivedImage.getLetency());
        }
    }

    /**
     * This method will rotate a given image. Src:
     * http://www.exampledepot.com/egs/java.awt.image/Image2Buf.html
     *
     * @param img    This is the image.
     * @param degree This is the angle in degrees.
     * @return A rotated image will be returned.
     */
    public static Image rotateImage(Image img, double degree) {
        BufferedImage bufImg = toBufferedImage(img);
        double angle = Math.toRadians(degree);

        return tilt(bufImg, angle);
    }

    /**
     * This method will tilt a given image across a given angle. Src:
     * http://www.exampledepot.com/egs/java.awt.image/Image2Buf.html
     *
     * @param image This is the image.
     * @param angle This is the angle in Radians.
     * @return A rotated image will be returned.
     */
    public static BufferedImage tilt(BufferedImage image, double angle) {
        double sin = Math.abs(Math.sin(angle)), cos = Math.abs(Math.cos(angle));
        int w = image.getWidth(), h = image.getHeight();
        int neww = (int) Math.floor(w * cos + h * sin), newh = (int) Math.floor(h * cos + w * sin);
        GraphicsConfiguration gc = getDefaultConfiguration();
        BufferedImage result = gc.createCompatibleImage(neww, newh, Transparency.TRANSLUCENT);
        Graphics2D g = result.createGraphics();
        g.translate((neww - w) / 2, (newh - h) / 2);
        g.rotate(angle, w / 2, h / 2);
        g.drawRenderedImage(image, null);
        g.dispose();
        return result;
    }

    /**
     * This method will return the graphics configuration. Src:
     * http://www.exampledepot.com/egs/java.awt.image/Image2Buf.html
     *
     * @return
     */
    public static GraphicsConfiguration getDefaultConfiguration() {
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        GraphicsDevice gd = ge.getDefaultScreenDevice();
        return gd.getDefaultConfiguration();
    }

    // http://www.exampledepot.com/egs/java.awt.image/Image2Buf.html
    // An Image object cannot be converted to a BufferedImage object.
    // The closest equivalent is to create a buffered image and then draw the image
    // on the buffered image.
    // This example defines a method that does this.

    // This method returns a buffered image with the contents of an image
    public static BufferedImage toBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage) image;
        }

        // This code ensures that all the pixels in the image are loaded
        image = new ImageIcon(image).getImage();

        // Determine if the image has transparent pixels; for this method's
        // implementation, see e661 Determining If an Image Has Transparent Pixels
        boolean hasAlpha = hasAlpha(image);

        // Create a buffered image with a format that's compatible with the screen
        BufferedImage bimage = null;
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        try {
            // Determine the type of transparency of the new buffered image
            int transparency = Transparency.OPAQUE;
            if (hasAlpha) {
                transparency = Transparency.BITMASK;
            }

            // Create the buffered image
            GraphicsDevice gs = ge.getDefaultScreenDevice();
            GraphicsConfiguration gc = gs.getDefaultConfiguration();
            bimage = gc.createCompatibleImage(image.getWidth(null), image.getHeight(null), transparency);
        } catch (HeadlessException e) {
            // The system does not have a screen
        }

        if (bimage == null) {
            // Create a buffered image using the default color model
            int type = BufferedImage.TYPE_INT_RGB;
            if (hasAlpha) {
                type = BufferedImage.TYPE_INT_ARGB;
            }
            bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), type);
        }

        // Copy image to buffered image
        Graphics g = bimage.createGraphics();

        // Paint the image onto the buffered image
        g.drawImage(image, 0, 0, null);
        g.dispose();

        return bimage;
    }

    // http://www.exampledepot.com/egs/java.awt.image/HasAlpha.html
    // This method returns true if the specified image has transparent pixels
    public static boolean hasAlpha(Image image) {
        // If buffered image, the color model is readily available
        if (image instanceof BufferedImage) {
            BufferedImage bimage = (BufferedImage) image;
            return bimage.getColorModel().hasAlpha();
        }

        // Use a pixel grabber to retrieve the image's color model;
        // grabbing a single pixel is usually sufficient
        PixelGrabber pg = new PixelGrabber(image, 0, 0, 1, 1, false);
        try {
            pg.grabPixels();
        } catch (InterruptedException e) {
        }

        // Get the image's color model
        ColorModel cm = pg.getColorModel();
        return cm.hasAlpha();
    }

}
