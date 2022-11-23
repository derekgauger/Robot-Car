package edu.msoe.se3910.robot.core;

public interface iLineSensorController {
    public static final int LINE_CONTROL_QUEUE =  0x00000003;
    public void enableLineSensing();
    public void disableLineSensing();

    public void enableLineFollowing();
    public void disableLineFollowing();
}
