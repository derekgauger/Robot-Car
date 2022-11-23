package edu.msoe.se3910.robot.gui;

import edu.msoe.se3910.robot.core.iLineSensorController;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;


/**
 * This class defines the controls which will adjust how the robot moves.
 * @author schilling
 *
 */
public class LineTrackerGUIPanel extends JPanel {

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    public LineTrackerGUIPanel(iLineSensorController lsc, JFrame jfMainWindow) {
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(0,1));

        BorderLayout myLayout = new BorderLayout();
        this.setLayout(myLayout);

        JCheckBox  enableLineSTop = new JCheckBox("Line Sensing");
        JCheckBox  enableLineFollowing = new JCheckBox("Line Following");
        enableLineFollowing.setEnabled(false);


        enableLineSTop.addItemListener(new ItemListener(){
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (enableLineSTop.isSelected()==true)
                {
                    lsc.enableLineSensing();
                    enableLineFollowing.setEnabled(true);
                }
                else
                {
                    lsc.disableLineSensing();
                    enableLineFollowing.setSelected(false);
                    enableLineFollowing.setEnabled(false);
                }
            }
        });

        enableLineFollowing.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (enableLineFollowing.isSelected()==true)
                {
                    lsc.enableLineFollowing();
                }
                else
                {
                    lsc.disableLineFollowing();
                }
            }
        });
        mainPanel.add(enableLineSTop);
        mainPanel.add(enableLineFollowing);
        add(mainPanel, BorderLayout.CENTER);

    }
}
