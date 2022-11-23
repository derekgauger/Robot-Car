import java.io.IOException;
import javax.swing.WindowConstants;

import edu.msoe.se3910.imageTransmit.pcgui.FramedStreamedImage;
import edu.msoe.se3910.robot.core.*;

import edu.msoe.se3910.robot.gui.GUI;

public class Main {

	/**
	 * the entry point for this program
	 *
	 * @param args
	 */
	public static void main(String args[]) throws IOException, InterruptedException {
		int port = 9000;
		if (args.length != 1) {
			System.err.println("Usage: <Program Name> <Port>");
            System.err.println("Assuming port 9000");
        } else {
            port = Integer.parseInt(args[0]);
        }
			iNetworkController nwc = new NetworkController();
			Robot r = new Robot(nwc);
			WallSEHornController hc = new WallSEHornController(nwc);
			r.setHc(hc);
			WallSELineController lc = new WallSELineController(nwc);
			r.setLsc(lc);
			r.initiateRobotOperation();

			@SuppressWarnings("unused")
			GUI programGUI = new GUI(r, nwc);


			// TODO Restore this. FramedStreamedImage frame = new FramedStreamedImage(port, System.out);
			FramedStreamedImage frame = new FramedStreamedImage(port, null);
 frame.setTitle("Receiving Images on Port " + port);

			frame.setVisible(true);
			frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

			frame.startReceivingImages();
			frame.waitToStopReceivingImages();

    }
}
