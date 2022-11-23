package edu.msoe.se3910.robot.cmdui;

import java.util.Observable;
import java.util.Observer;

import edu.msoe.se3910.robot.core.iOdometer;

/**
 * This class will print out the distance traveled by the robot to the console as an observer.
 * @author schilling
 *
 */
public class OdometerDisplay implements Observer {
	private iOdometer ov = null;

	public OdometerDisplay(iOdometer ov) {
		this.ov = ov;
	}

	@Override
	public void update(Observable obs, Object arg) {
		if (obs == ov) {
			System.out.printf("Odometer: %.1f inches\n", ov.getOdometerReading());
		}
	}
}
