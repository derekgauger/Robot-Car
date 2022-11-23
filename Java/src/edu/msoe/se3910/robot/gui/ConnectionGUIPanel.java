package edu.msoe.se3910.robot.gui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.UnknownHostException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

import edu.msoe.se3910.robot.core.iNetworkController;

public class ConnectionGUIPanel extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
	 * This constructor will instantiate a new instance of the odometer display.
	 * It is linked through an observer pattern to the odometer subject.
	 * 
	 * @param nwc
	 * @param ov
	 *            This is the odometer which is being observed as the subject.
	 */
	public ConnectionGUIPanel(final iNetworkController nwc, final JFrame jfMainWindow) {
		this.setLayout(new BorderLayout());

		final JTextField textField = new JTextField("192.168.0.1");
		

		final JButton connectDisconnect = new JButton("Connect");
		connectDisconnect.setFocusable(false);

		connectDisconnect.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				if (nwc.getConnectionStatus() == false) {
					nwc.setIPAddress(textField.getText());
					try {
						nwc.connect();
						connectDisconnect.setText("Disconnect");
						textField.setEnabled(false);
						textField.setFocusable(false);
					} catch (UnknownHostException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				else if (nwc.getConnectionStatus()==true)
				{
					nwc.disconnect();
					connectDisconnect.setText("Connect");
					textField.setEnabled(true);
					textField.setFocusable(true);
				}

				jfMainWindow.requestFocus();
			}
		});
		
		textField.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent event) {
				connectDisconnect.doClick();
			}
			
		});

		this.add(textField, BorderLayout.CENTER);
		this.add(connectDisconnect, BorderLayout.EAST);
	}
}
