package edu.msoe.se3910.robot.gui;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.Observable;
import java.util.Observer;

import javax.swing.*;

import edu.msoe.se3910.robot.core.iDistanceMeasurement;
import edu.msoe.se3910.robot.core.iNetworkController;


/**
 * This class will display the output of an odometer reading.
 * @author schilling
 *
 */
public class DistanceDisplayPanel extends JPanel implements Observer {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JLabel currentDistance;
	private JLabel averageDistance;
	private JLabel maxDistance;
	private JLabel minDistance;
	private iDistanceMeasurement ov = null;

	private boolean paused=false;

	/**
	 * This constructor will instantiate a new instance of the odometer display.  It is linked through an observer pattern to the odometer subject. 
	 * @param ov This is the odometer which is being observed as the subject.
	 */
	public DistanceDisplayPanel(final iNetworkController nwc, iDistanceMeasurement ov) {
		this.ov = ov;
		this.setLayout(new BorderLayout());

		JPanel internalPanel = new JPanel();
		internalPanel.setLayout(new GridLayout(4,1));

		this.currentDistance = new JLabel("CURR: 0000.00 in");
		this.currentDistance.setFont(new Font("NI7SEG", Font.PLAIN, 24));
		this.currentDistance.setForeground(Color.GREEN);
		this.currentDistance.setBackground(Color.GRAY);
		super.setBackground(Color.GRAY);
		internalPanel.add(this.currentDistance);

		this.averageDistance = new JLabel("AVE: 0000.00 in");
		this.averageDistance.setFont(new Font("NI7SEG", Font.PLAIN, 24));
		this.averageDistance.setForeground(Color.GREEN);
		this.averageDistance.setBackground(Color.GRAY);
		super.setBackground(Color.GRAY);
		internalPanel.add(this.averageDistance);

		this.maxDistance = new JLabel("MAX: 0000.00 in");
		this.maxDistance.setFont(new Font("NI7SEG", Font.PLAIN, 24));
		this.maxDistance.setForeground(Color.GREEN);
		this.maxDistance.setBackground(Color.GRAY);
		super.setBackground(Color.GRAY);
		internalPanel.add(this.maxDistance);

		this.minDistance = new JLabel("MIN: 0000.00 in");
		this.minDistance.setFont(new Font("NI7SEG", Font.PLAIN, 24));
		this.minDistance.setForeground(Color.GREEN);
		this.minDistance.setBackground(Color.GRAY);
		super.setBackground(Color.GRAY);
		internalPanel.add(this.minDistance);

		final JButton connectDisconnect = new JButton("Pause/Unpause");
		connectDisconnect.setFocusable(false);

		connectDisconnect.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				paused = !paused;
			}
		});
		this.add(internalPanel, BorderLayout.CENTER);
		this.add(connectDisconnect, BorderLayout.EAST);

	}

	@Override
	public void update(Observable obs, Object arg) {
		if (obs == ov && !paused) {
			DecimalFormat myFormatter = new DecimalFormat("###0.00");
			String text = "CURR " + myFormatter.format(0.0393701 * ov.getcurrentDistanceReading())+ " in ";
			this.currentDistance.setText(text);

			text ="AVE " + myFormatter.format(0.0393701 * ov.getAveDistanceReading())+ " in ";
			this.averageDistance.setText(text);

			text ="MAX " + myFormatter.format(0.0393701 * ov.getMaxDistanceReading())+ " in ";
			this.maxDistance.setText(text);

			text = "MIN " + myFormatter.format(0.0393701 * ov.getMinDistanceReading())+ " in ";
			this.minDistance.setText(text);

		}
	}
}
