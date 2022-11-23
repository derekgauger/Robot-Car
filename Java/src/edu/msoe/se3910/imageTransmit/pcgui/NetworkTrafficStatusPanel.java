package edu.msoe.se3910.imageTransmit.pcgui;

import javax.swing.*;
import java.awt.*;

public class NetworkTrafficStatusPanel extends JPanel {
    private JProgressBar udpPacketsReceivedStatus = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
    private JProgressBar udpAvgPacketsReceivedStatus = new JProgressBar(JProgressBar.VERTICAL, 0, 100);

    private int updateCount=0;
    private int total=0;

    public NetworkTrafficStatusPanel() {
        this.setLayout(new GridLayout(0, 2));
        this.add(new JLabel("Cur: "));
        this.add(udpPacketsReceivedStatus);
        udpPacketsReceivedStatus.setStringPainted(true);
        this.add(new JLabel("Overall: "));

        this.add(udpAvgPacketsReceivedStatus);
        udpAvgPacketsReceivedStatus.setStringPainted(true);
    }

    public void setcurrentStatusValue(int percentReceived) {
        updateCount++;
        total+= percentReceived;
        udpPacketsReceivedStatus.setValue(percentReceived);
        udpAvgPacketsReceivedStatus.setValue((total/updateCount));
    }
}
