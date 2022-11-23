package edu.msoe.se3910.robot.gui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

import edu.msoe.se3910.robot.core.iHornController;
import edu.msoe.se3910.robot.core.iRobotPropulsionController;


/**
 * This class defines the controls which will adjust how the robot moves.
 * @author schilling
 *
 */
public class HornGUIPanel extends JPanel {

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
    public HornGUIPanel(iHornController hc, JFrame jfMainWindow) {
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(3,0));

        BorderLayout myLayout = new BorderLayout();
        this.setLayout(myLayout);

        JButton silence = new JButton("Mute");
        JButton pulse = new JButton("Pulse");
        JButton sound = new JButton("Sound");

        silence.setFocusable(false);
        pulse.setFocusable(false);
        sound.setFocusable(false);

        silence.addActionListener(new ActionListener(){
            @Override
            public void actionPerformed(ActionEvent e) {
                hc.muteHorn();
            }
        });

        sound.addActionListener(new ActionListener(){
            @Override
            public void actionPerformed(ActionEvent e) {
                hc.soundHorn();
            }
        });

        pulse.addActionListener(new ActionListener(){
            @Override
            public void actionPerformed(ActionEvent e) {
                hc.pulseHorn(600, 300);
            }
        });


        mainPanel.add(silence);
        mainPanel.add(pulse);
        mainPanel.add(sound);

        add(mainPanel, BorderLayout.CENTER);
    }
}
