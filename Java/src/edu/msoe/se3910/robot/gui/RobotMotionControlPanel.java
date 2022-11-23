package edu.msoe.se3910.robot.gui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import edu.msoe.se3910.robot.core.iRobotPropulsionController;


/**
 * This class defines the controls which will adjust how the robot moves.
 * @author schilling
 *
 */
public class RobotMotionControlPanel extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private iRobotPropulsionController motorController;

	/**
	 * This constructor will instantiate a new instance of the robot motion
	 * controller, which will control the operation of the robot.
	 * 
	 * @param pmctrl
	 *            This is the instance of the robot propulsion motor controller
	 *            that is to be controlled by this panel.
	 */
	public RobotMotionControlPanel(iRobotPropulsionController pmctrl, JFrame jfMainWindow) {
		motorController = pmctrl;

		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new GridLayout(4,0));

		BorderLayout myLayout = new BorderLayout();
		this.setLayout(myLayout);

		JButton forwardButton = new JButton("/\\ \'W\'");
		JButton leftForwardButton = new JButton("|\\- \'Q\'");
		JButton rightForwardButton = new JButton("-/| \'E\'");
		
		JButton leftButton = new JButton("<- \'A\'");
		JButton rightButton = new JButton("-> \'D\'");
		JButton backwardButton = new JButton("\\/ \'S\'");
		JButton leftBackwardButton = new JButton("|//_ \'Z\'");
		JButton rightBackwardButton = new JButton("_\\| \'C\'");
		
		JButton stopButton = new JButton("STOP");
		
		forwardButton.setFocusable(false);
		leftForwardButton.setFocusable(false);
		rightForwardButton.setFocusable(false);
		leftButton.setFocusable(false);
		rightButton.setFocusable(false);
		backwardButton.setFocusable(false);
		leftBackwardButton.setFocusable(false);
		rightBackwardButton.setFocusable(false);
		stopButton.setFocusable(false);

		JPanel steeringPanel = new JPanel();
		final JSlider steeringControl =  new JSlider(JSlider.HORIZONTAL, -100, 100, 0);
		steeringControl.setMajorTickSpacing(10);
		steeringControl.setPaintTicks(true);
		steeringPanel.add(steeringControl);
		steeringControl.setFocusable(false);

		steeringControl.addChangeListener(new ChangeListener(){

			@Override
			public void stateChanged(ChangeEvent e) {
				motorController.setSteeringValue(steeringControl.getValue());
			}});

		JPanel topPanel = new JPanel();
		topPanel.setLayout(new GridLayout(0,3));
		topPanel.add(leftForwardButton);
		topPanel.add(forwardButton);
		topPanel.add(rightForwardButton);
		
		JPanel bottomPanel = new JPanel();
		bottomPanel.setLayout(new GridLayout(0,3));
		bottomPanel.add(leftBackwardButton);
		bottomPanel.add(backwardButton);
		bottomPanel.add(rightBackwardButton);

		JPanel middlePanel = new JPanel();
		middlePanel.setLayout(new GridLayout(0,3));
		middlePanel.add(leftButton);
		middlePanel.add(stopButton);
		middlePanel.add(rightButton);

		mainPanel.add(steeringPanel);
		mainPanel.add(topPanel);
		mainPanel.add(middlePanel);
		mainPanel.add(bottomPanel);


		add(mainPanel, BorderLayout.WEST);
		final JSlider speedControl =  new JSlider(JSlider.VERTICAL, 0,
				100, 50);
		speedControl.setMajorTickSpacing(5);
		speedControl.setPaintTicks(true);
		speedControl.setFocusable(false);
		
		final JLabel speedLabel = new JLabel("<html>Robot Speed<br/>" + speedControl.getValue() + "%</html>");
		
		add(speedLabel, BorderLayout.CENTER);

		add(speedControl, BorderLayout.EAST);

		speedControl.addChangeListener(new ChangeListener(){

			@Override
			public void stateChanged(ChangeEvent e) {
				motorController.setVelocity(speedControl.getValue());
				speedLabel.setText("<html>Robot Speed<br/>" + speedControl.getValue() + "%</html>");
			}});


		stopButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.stopRobotMotion();
			}});


		leftButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.turnRobotLeft();
			}});
		rightButton.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.turnRobotRight();
			}});

		forwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotForward();
			}});
		leftForwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotForwardLeft();
			}});

		rightForwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotForwardRight();
			}});

		leftBackwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotBackwardLeft();;
			}});
	
		
		rightBackwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotBackwardRight();;
			}});

		

		backwardButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				motorController.setVelocity(speedControl.getValue());
				motorController.driveRobotBackward();
			}});
		
		jfMainWindow.addKeyListener(new KeyListener(){
			@Override
			public void keyPressed(KeyEvent key) {
				switch(key.getKeyChar()){
				case('q'):
					motorController.driveRobotForwardLeft();
					break;
				case('w'):
					motorController.driveRobotForward();
					break;
				case('e'):
					motorController.driveRobotForwardRight();
					break;
				case('a'):
					motorController.turnRobotLeft();
					break;
				case('d'):
					motorController.turnRobotRight();
					break;
				case('z'):
					motorController.driveRobotBackwardLeft();;
					break;
				case('s'):
					motorController.driveRobotBackward();
					break;
				case('c'):
					motorController.driveRobotBackwardRight();;
					break;
				}
			}

			@Override
			public void keyReleased(KeyEvent key) {
				if(key.getKeyChar() == 'q' ||
						key.getKeyChar() == 'w' ||
						key.getKeyChar() == 'e' ||
						key.getKeyChar() == 'a' ||
						key.getKeyChar() == 's' ||
						key.getKeyChar() == 'd' ||
						key.getKeyChar() == 'z' ||
						key.getKeyChar() == 'c') {
					motorController.stopRobotMotion();
				}
			}

			@Override
			public void keyTyped(KeyEvent arg0) {}
		});
		
		jfMainWindow.addMouseWheelListener(new MouseWheelListener(){

			@Override
			public void mouseWheelMoved(MouseWheelEvent wheel) {
				int newSpeed;
				
				if((wheel.getModifiers() & InputEvent.CTRL_MASK) == InputEvent.CTRL_MASK){
					newSpeed = speedControl.getValue() - (wheel.getWheelRotation() * 5);
				}else{
					newSpeed = speedControl.getValue() - wheel.getWheelRotation();
				}
				
				System.out.println("newSpeed is: " + newSpeed);
				
				speedControl.setValue(newSpeed);
			}
			
		});
	}
}
