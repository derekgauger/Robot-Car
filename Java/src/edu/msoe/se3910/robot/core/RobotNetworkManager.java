package edu.msoe.se3910.robot.core;

import java.io.IOException;

public class RobotNetworkManager implements Runnable {

    private Object[] SyncrhonizationObjects;
    private int [] messages;

    private iNetworkController nwInterface;

    boolean keepGoing = true;

    public RobotNetworkManager(int numberOfQueues, iNetworkController nwInterface)
    {
        this.nwInterface = nwInterface;
        SyncrhonizationObjects = new Object[numberOfQueues];
        messages = new int[numberOfQueues];

        for (int index = 0; index < numberOfQueues; index++)
        {
            SyncrhonizationObjects[index] = new Object();
            messages[index] = 0;
        }
    }

    public int obtainNextMessage(int queue)
    {
        int retVal = 0;
        synchronized(SyncrhonizationObjects[queue-1])
        {
            try {
                SyncrhonizationObjects[queue-1].wait();
                retVal = messages[queue-1];
            } catch (InterruptedException e) {
                // TODO Handle this better.
            }
        }
        return retVal;
    }

    private void processMessage(int queue, int message)
    {
        synchronized(SyncrhonizationObjects[queue-1])
        {
            messages[queue-1] = message;
            SyncrhonizationObjects[queue-1].notify();
        }
    }

    @Override
    public void run() {
    while (keepGoing)
    {
        try {
            int[] response = nwInterface.receiveMessage();
            if (response[0]>0) {
                processMessage(response[0], response[1]);
            }
            Thread.yield();
        } catch (IOException e) {
            // Ignore the error for right now.
            //TODO Handle this better.

        }

    }
    }


}
