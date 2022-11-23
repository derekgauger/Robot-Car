package edu.msoe.se3910.robot.core;

import java.util.Observer;

/**
 * This class defines the various pieces of a robot. It contains all of the
 * necessary items as well as allows for devices to obtain various pieces of
 * information about the robot.
 * 
 * @author schilling
 * 
 */
public class Robot {
	private iNetworkController nc;
	private iRobotPropulsionController pc;
	private RobotDistanceMeasurementController rdmc;
	private RobotNetworkManager rnm;
	private iHornController hc;
	private iLineSensorController lsc;

	/**
	 * This method will instantiate a new instance of the robot.
	 */
	public Robot(iNetworkController nc) {
		this.nc = nc;
		pc = new WallSEMotorController(nc);
		rnm = new RobotNetworkManager(1, nc);
		rdmc = new RobotDistanceMeasurementController(rnm);
	}
	public void subscribeToDistanceUpdates(Observer obs)
	{
		rdmc.addObserver(obs);
	}

	/**
	 * This method will cause the robot to start operating in a normal fashion.
	 * It will start all subthreads with appropriate delays in between to ensure
	 * reliable operation.
	 */
	public void initiateRobotOperation() {
	Thread t1 = new Thread(rnm);
	t1.start();
	Thread t2 = new Thread(rdmc);
	t2.start();
	}

	public iNetworkController getNc() {
		return nc;
	}

	public iRobotPropulsionController getPc() {
		return pc;
	}

	public void setNc(iNetworkController nc) {
		this.nc = nc;
	}

	public void setPc(iRobotPropulsionController pc) {
		this.pc = pc;
	}

	public iHornController getHc() {
		return hc;
	}

	public void setHc(iHornController hc) {
		this.hc = hc;
	}

	public iLineSensorController getLsc() {
		return lsc;
	}
	public RobotDistanceMeasurementController getRdmc() {
		return rdmc;
	}

	public void setLsc(iLineSensorController lsc) {
		this.lsc = lsc;
	}
}
