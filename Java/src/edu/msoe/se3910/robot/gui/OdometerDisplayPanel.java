package edu.msoe.se3910.robot.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.text.DecimalFormat;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JLabel;
import javax.swing.JPanel;

import edu.msoe.se3910.robot.core.iOdometer;


/**
 * This class will display the output of an odometer reading.
 * @author schilling
 *
 */
public class OdometerDisplayPanel extends JPanel implements Observer {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JLabel mileage;
	private iOdometer ov = null;

	/**
	 * This constructor will instantiate a new instance of the odometer display.  It is linked through an observer pattern to the odometer subject. 
	 * @param ov This is the odometer which is being observed as the subject.
	 */
	public OdometerDisplayPanel(iOdometer ov) {
		this.ov = ov;
		this.setLayout(new BorderLayout());

		this.mileage = new JLabel("OD 000000.00");
		this.mileage.setFont(new Font("NI7SEG", Font.PLAIN, 24));
		this.mileage.setForeground(Color.GREEN);
		this.mileage.setBackground(Color.GRAY);
		super.setBackground(Color.GRAY);
		this.add(this.mileage, BorderLayout.CENTER);
	}

	@Override
	public void update(Observable obs, Object arg) {
		if (obs == ov) {
			DecimalFormat myFormatter = new DecimalFormat("#####.##");
			String text = "ODOM " + myFormatter.format(ov.getOdometerReading()/12)+ "FEET";
			this.mileage.setText(text);
		}
	}
}
