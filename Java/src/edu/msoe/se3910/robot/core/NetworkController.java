package edu.msoe.se3910.robot.core;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class NetworkController implements iNetworkController {

	private Socket socket;
	private String ip;
	private DataOutputStream out;
	private DataInputStream in;
	private boolean connected = false;
	private int messageSequenceNumber=0;
	private long timestamp=0;

	private static final int COMMAND_MSG_TYPE = 0x09;
	
	

	public NetworkController() {
		super();
	}

	@Override
	public void setIPAddress(String ip) {
		this.ip = ip;
	}

	@Override
	public void connect() throws UnknownHostException, IOException {
		socket = new Socket(ip, 9090);
		out = new DataOutputStream(socket.getOutputStream());
		in = new DataInputStream(socket.getInputStream());
		connected = true;
	}

	@Override
	public void disconnect() {
		try {
			out.write(0xFFFFFFFF);
			out.write(0x00000000);
			out.write(0xFFFFFFFF);

			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		connected = false;
		out = null;
	}

	@Override
	public boolean getConnectionStatus() {
		return connected;
	}

	@Override
	public void sendMessage(int destination, int message) throws IOException {
		if (out != null) {

			// The msg sequence number is a number that keeps increasing each time a message is sent.
			int msgID = messageSequenceNumber++;
			long timestamp = System.currentTimeMillis();
			int timestampHigh = ((int)((timestamp & 0xFFFFFFFF00000000l)>> 32));
			int timestampLow = ((int)((timestamp & 0x00000000FFFFFFFFl)));
			int messageType = COMMAND_MSG_TYPE;

			int checksum = msgID ^ timestampHigh ^ timestampLow ^ messageType ^ message ^ destination;
			System.out.printf("Sent: MID %x TS %x %x T %x D %x MSG %x CK %x\n", msgID, timestampHigh, timestampLow, messageType, destination, message, checksum);
			out.writeInt(msgID);
			out.writeInt(timestampHigh);
			out.writeInt(timestampLow);
			out.writeInt(messageType);
			out.writeInt(destination);
			out.writeInt(message);
			out.writeInt(checksum);
			out.flush();
		}
	}

	@Override
	public int[] receiveMessage()  throws IOException {
		int retVal[] = {0,0};
		if (in != null)
		{
			boolean valid = false;

			int valuesRead[]={0,0,0};

			while (!valid) {
				// Move the values up by 1 location in the array.
				valuesRead[0] = valuesRead[1];
				valuesRead[1] = valuesRead[2];

				// Read in an int.
				valuesRead[2] = in.readInt();
				// Verify the checksum and set return values if correct.
				if (valuesRead[2] == (valuesRead[1] ^ valuesRead[0])) {
					retVal[0] = valuesRead[0];
					retVal[1] = valuesRead[1];
					valid = true;
				}
			}
		}
		return retVal;
	}
}
