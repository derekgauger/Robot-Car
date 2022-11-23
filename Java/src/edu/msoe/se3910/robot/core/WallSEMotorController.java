package edu.msoe.se3910.robot.core;

import java.io.IOException;

public class WallSEMotorController implements iRobotPropulsionController {

	private iNetworkController network;

	private static int MOTORDIRECTIONBITMAP = 0x20000000;
	private static int FORWARD = 0x00000001;
	private static int BACKWARD = 0x00000002;
	private static int LEFT = 0x00000004;
	private static int RIGHT = 0x00000008;
	private static int STOP = 0x00000010;

	private static int SPEEDDIRECTIONBITMAP = 0x40000000;

	private static int STEERINGOFFSETBITMAP = 0x10000000;

	public WallSEMotorController(iNetworkController nw) {
		network = nw;
	}

	@Override
	public void driveRobotForward() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | FORWARD);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void driveRobotBackward() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | BACKWARD);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void turnRobotRight() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | RIGHT);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void turnRobotLeft() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | LEFT);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void stopRobotMotion() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | STOP);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void setVelocity(int velocity) {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, SPEEDDIRECTIONBITMAP | (10*velocity));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public int getVelocity() {
		return 0;
	}

	@Override
	public void driveRobotForwardLeft() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | LEFT | FORWARD);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

	@Override
	public void driveRobotForwardRight() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | RIGHT | FORWARD );
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

	@Override
	public void driveRobotBackwardLeft() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | LEFT | BACKWARD);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void driveRobotBackwardRight() {
		try {
			network.sendMessage(iRobotController.MOTOR_CONTROL, MOTORDIRECTIONBITMAP | RIGHT | BACKWARD);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void setSteeringValue(int value) {
		if (value >= -100 && value <= 100) {
			try {
				network.sendMessage(iRobotController.MOTOR_CONTROL, STEERINGOFFSETBITMAP | (value + 100));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
