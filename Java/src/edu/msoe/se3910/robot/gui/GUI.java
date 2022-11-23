package edu.msoe.se3910.robot.gui;

/**
 * This file is not copyrighted.  Others are free to copy, modify, or redistribute this file as is seen fit.
 * 
 * @author hornick
 * $Header: d:\cvs/webpage/msoe/se1020/GUI.java,v 1.1 2007/12/13 22:31:00 schilling Exp $
 * $Name:  $ 
 * $Revision: 1.1 $ 
 * $Date: 2007/12/13 22:31:00 $ 
 * $Log: GUI.java,v $
 * Revision 1.1  2007/12/13 22:31:00  schilling
 * Added archived to MSOE CVS repository.
 *
 * Revision 1.1  2007/11/30 23:01:57  wws
 * Updated SE1020 material on the website, as well as uploaded notes for SE-4831.
 *
 *
 *
 **/
import java.awt.Color;
import java.awt.Container;

import javax.swing.JFrame;

import edu.msoe.se3910.robot.core.Robot;
import edu.msoe.se3910.robot.core.iNetworkController;

public class GUI {
	private Container contentPane;
	private JFrame jfMainWindow; // main window

	private DistanceDisplayPanel distanceDisplay;
	private RobotMotionControlPanel motionControlUI;
	private ConnectionGUIPanel connectionPanel;
	private HornGUIPanel hgp;
	private LineTrackerGUIPanel ltgp;

	/**
	 * constructor for the GUI class creates the JFrame window and subcomponents
	 */
	public GUI(Robot robot, iNetworkController nwc) {
		// create a JFrame window and set its properties
		jfMainWindow = new JFrame("MSOE WALL-SE");
		jfMainWindow.setSize(420, 315); // window size
		jfMainWindow.setLocation(10, 10); // window location
		jfMainWindow.setResizable(true); // not resizeable

		/* !!!! */jfMainWindow.setVisible(true); // make visible!!!

		// EXIT_ON_CLOSE destroys all windows
		jfMainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// get the container that is part of the JFrame
		contentPane = jfMainWindow.getContentPane();

		// we're using absolute positioning
		contentPane.setLayout(null);

		// override default background color
		contentPane.setBackground(Color.GRAY);

		motionControlUI = new RobotMotionControlPanel(robot.getPc(), jfMainWindow);
		motionControlUI.setBounds(5, 110, 300, 100);

		distanceDisplay = new DistanceDisplayPanel(nwc, robot.getRdmc());
		distanceDisplay.setBounds(5, 5, 384, 100);
		robot.subscribeToDistanceUpdates(distanceDisplay);

		connectionPanel = new ConnectionGUIPanel(nwc, jfMainWindow);
		connectionPanel.setBounds(5, 220, 250, 50);

		hgp =new HornGUIPanel(robot.getHc(), jfMainWindow);
		hgp.setBounds(310, 110, 80, 100);

		ltgp = new LineTrackerGUIPanel(robot.getLsc(), jfMainWindow);
		ltgp.setBounds(260, 220, 120, 30);
			contentPane.add(this.distanceDisplay);
		contentPane.add(motionControlUI);
		contentPane.add(connectionPanel);
		contentPane.add(hgp);
		contentPane.add(ltgp);
		contentPane.add(this.distanceDisplay);
		
		// always call validate() after adding components
		// if the window is already visible
		contentPane.validate();
		// ...and this too; forces a refresh
		contentPane.repaint();
		contentPane.setFocusable(true);
		this.setDashColor(Color.CYAN);
	}

	/**
	 * This method will set the background color of all components on the display as is appropriate.
	 * @param c This is the color to be set.
	 */
	private void setDashColor(Color c) {
		this.contentPane.setBackground(c);
		this.jfMainWindow.setBackground(c);
	}

}
