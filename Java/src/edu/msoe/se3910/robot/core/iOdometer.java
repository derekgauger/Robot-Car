package edu.msoe.se3910.robot.core;

/**
 * This class defines the interface used to access the robots odometer. The
 * odometer stores the distance traveled by the robot in inches.
 * 
 * @author schilling
 * 
 */
public interface iOdometer {

	/**
	 * This method will return the odometer reading for the robot. The odometer
	 * reading represents the distance traveled by the robot since it was first
	 * started.
	 * 
	 * @return The odometer reading will be returned. This represents the
	 *         number of inches that the robot has moved in a forward direction
	 *         since the start.
	 */
	public abstract double getOdometerReading();

}