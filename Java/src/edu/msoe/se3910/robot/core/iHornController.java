package edu.msoe.se3910.robot.core;

public interface iHornController {
    public static final int HORN_CONTROL_QUEUE =  0x00000002;

    public void soundHorn();
    public void muteHorn();
    public void pulseHorn(int period, int onTime);
}
