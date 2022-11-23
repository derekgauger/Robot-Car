package edu.msoe.se3910.robot.core;

import java.io.IOException;

public class WallSEHornController implements iHornController {
    private iNetworkController network;



    /**
     * This indicates that the horn sound is to be muted.
     */
    private static int HORN_MUTE_COMMAND = (0x40000000);

/**
 * This command indicates that the horn is to sound continuously.
 */
private static int HORN_SOUND_COMMAND =(0x20000000);

/**
 *  This indicates that the command received is a command to pulse the horn.
 *  The lowest 12 bits indicate the period for the horn sound in ms.
 *  The next 12 bits indicate the time that the horn is to sound in ms.
 *  The robot may approximate these periods based upon the periodic task rate selected.
 */
private static int HORN_PULSE_COMMAND =(0x10000000);


    public WallSEHornController(iNetworkController nw) {
        network = nw;
    }


    @Override
    public void soundHorn() {
        try {
            network.sendMessage(HORN_CONTROL_QUEUE, HORN_SOUND_COMMAND);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void muteHorn() {
        try {
            network.sendMessage(HORN_CONTROL_QUEUE, HORN_MUTE_COMMAND);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void pulseHorn(int period, int onTime) {
        try {
            network.sendMessage(HORN_CONTROL_QUEUE, HORN_PULSE_COMMAND | (300 << 12) | (600) );
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
