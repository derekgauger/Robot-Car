package edu.msoe.se3910.robot.core;

/**
 * This class defines the interface used to access the robots distance measurement system.
 * 
 * @author schilling
 * 
 */
public interface iDistanceMeasurement {
	 int getcurrentDistanceReading();
	 int getMaxDistanceReading();
	 int getMinDistanceReading();
	int getAveDistanceReading();

}