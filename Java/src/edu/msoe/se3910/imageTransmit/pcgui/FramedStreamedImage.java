package edu.msoe.se3910.imageTransmit.pcgui;

import java.awt.FlowLayout;
import java.io.IOException;
import java.io.PrintStream;
import java.net.InetAddress;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

import edu.msoe.se3910.imageTransmit.core.ConnectionObserver;

public class FramedStreamedImage extends JFrame implements ConnectionObserver {
    Thread myThread;
    StreamedImagePanel s;

    public FramedStreamedImage(int port, PrintStream log) throws IOException {
        this(port, 640, 360, log);
    }

    public FramedStreamedImage(int port, int width, int height, PrintStream log) throws IOException {
        super();

        s = new StreamedImagePanel(port, width, height);
        s.setLogStream(log);

        this.getContentPane().setLayout(new FlowLayout());
        this.getContentPane().add(s);
        this.pack();
        this.setVisible(true);
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        s.addObserver(this);

    }

    @Override
    public void connectionEstabolished(InetAddress src) {
        if (src != null) {

            if (!getTitle().equals(src.toString())) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        setTitle(src.toString());
                    }
                });
            }
        }
    }

    public void startReceivingImages() {
        myThread = new Thread(s);
        myThread.start();
    }

    public void waitToStopReceivingImages() throws InterruptedException {
        myThread.join();
    }

}
