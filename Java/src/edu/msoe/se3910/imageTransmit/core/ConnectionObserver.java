package edu.msoe.se3910.imageTransmit.core;
import java.net.InetAddress;

/**
 * This interface is used to indicate that a connection has been estabolished.
 * 
 * @author wws
 *
 */
public interface ConnectionObserver {
	/**
	 * This will be invoked when a connection has occurred.
	 * 
	 * @param inetAddress
	 *            This is the source of the connection.
	 */
	public void connectionEstabolished(InetAddress inetAddress);
}
