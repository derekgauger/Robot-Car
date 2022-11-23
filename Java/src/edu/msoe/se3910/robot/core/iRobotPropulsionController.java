package edu.msoe.se3910.robot.core;

public interface iRobotPropulsionController {

    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a forward direction. The speed will be determined by the motor
     * velocity.
     */
    public abstract void driveRobotForward();

    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a forward left direction. The speed will be determined by the motor
     * velocity.
     */
    public abstract void driveRobotForwardLeft();

    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a forward right direction. The speed will be determined by the motor
     * velocity.
     */
    public abstract void driveRobotForwardRight();


    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a backward direction. The speed will be determined by the
     * motor velocity.
     */
    public abstract void driveRobotBackward();

    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a backward left direction. The speed will be determined by the motor
     * velocity.
     */
    public abstract void driveRobotBackwardLeft();

    /**
     * This method will cause the motors to start running so as to propel the
     * vehicle in a backward right direction. The speed will be determined by the motor
     * velocity.
     */
    public abstract void driveRobotBackwardRight();

    /**
     * This method will set the steering parameter for the robot.  A value of 0 indicates the robot is to go straight.
     * A value of 100 indicates the robot is to steer to the right 100 %.  The left motor will run at full speed while
     * the right motor will be stopped.  A value of -100 will do the exact opposite.
     * @param value This is the steering value, between -100 and 100.
     */
    public abstract void setSteeringValue(int value);


    /**
     * This method will cause the robot to start turning to the right.
     */
    public abstract void turnRobotRight();

    /**
     * This method will cause the robot to start turning to the left.
     */
    public abstract void turnRobotLeft();

    /**
     * This method will stop the motors from running, resulting in them halting
     * in their current position.
     */
    public abstract void stopRobotMotion();

    /**
     * This method will set the velocity for the vehicle.
     *
     * @param motorSpeed This is the speed of the vehicle given in inches per minute.
     */
    public abstract void setVelocity(int velocity);

    /**
     * This method will return the set motor velocity from the robot.
     *
     * @return the velocity
     */
    public abstract int getVelocity();

}