package edu.msoe.se3910.imageTransmit.pcgui;

import javax.swing.*;
import java.awt.*;

public class DiagnosticControls extends JPanel {
    private JCheckBox printNewHeaders = new JCheckBox("Print New Frame Headers");
    private JCheckBox extraDebugInfo = new JCheckBox("Print Extra Debug Info");
    private JCheckBox retainImageRows = new JCheckBox("Retain Row Pixel Data");

    public DiagnosticControls()
    {
        this.setLayout(new FlowLayout());

        this.add(printNewHeaders);
        this.add(extraDebugInfo);
        this.add(retainImageRows);
    }

    public boolean isPrintNewHeadersActive()
    {
        return printNewHeaders.isSelected();
    }

    public boolean isExtraDebugInfoActive()
    {
        return extraDebugInfo.isSelected();
    }

    public boolean isRowPixelInformationToBeRetained() {return retainImageRows.isSelected();}
}
