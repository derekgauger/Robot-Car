package edu.msoe.se3910.robot.core;

import java.io.IOException;

public class WallSELineController implements iLineSensorController {
    private iNetworkController network;
    private static int ENABLE_LINE_SENSING = (0x40000000);
    private static int DISABLE_LINE_SENSING = (0x20000000);

    private static int ENABLE_LINE_FOLLOWING =  (0x04000000);
    private static int DISABLE_LINE_FOLLOWING = (0x02000000);

    public WallSELineController(iNetworkController nw) {
        network = nw;
    }

    @Override
    public void enableLineSensing() {
        try {
            network.sendMessage(LINE_CONTROL_QUEUE, ENABLE_LINE_SENSING);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void disableLineSensing() {
        try {
            network.sendMessage(LINE_CONTROL_QUEUE, DISABLE_LINE_SENSING);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void enableLineFollowing() {
        try {
            network.sendMessage(LINE_CONTROL_QUEUE, ENABLE_LINE_FOLLOWING);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void disableLineFollowing() {
        try {
            network.sendMessage(LINE_CONTROL_QUEUE, DISABLE_LINE_FOLLOWING);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
