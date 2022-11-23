package edu.msoe.se3910.robot.core;

import java.util.Observable;

public class RobotDistanceMeasurementController extends Observable implements Runnable, iDistanceMeasurement {
    private static final int DISTANCE_MEASUREMENT_QUEUE = 1;

    private static final int DISTANCEREADINGBITMAP = (0x40000000);
    private static final int CURRENTREADINGBITMAP =  (0x08000000);
    private static final int AVEREADINGBITMAP =      (0x04000000);
    private static final int MAXREADINGBITMAP =      (0x02000000);
    private static final int MINREADINGBITMAP =      (0x01000000);

    private RobotNetworkManager rnm;
    private boolean keepGoing = true;
    private int currentDistance = 0;
    private int aveDistance = 0;
    private int maxDistance = 0;
    private int minDistance = 0;

    public RobotDistanceMeasurementController(RobotNetworkManager rnm) {
        this.rnm = rnm;
    }

    public void setValue(int n) {
        int type = n & (DISTANCEREADINGBITMAP | CURRENTREADINGBITMAP | AVEREADINGBITMAP | MAXREADINGBITMAP | MINREADINGBITMAP);

        if (type == (DISTANCEREADINGBITMAP | CURRENTREADINGBITMAP)) {
            this.currentDistance = n & (~(DISTANCEREADINGBITMAP | CURRENTREADINGBITMAP));
            setChanged();
            notifyObservers();
        } else if (type == (DISTANCEREADINGBITMAP | AVEREADINGBITMAP)) {
            this.aveDistance = n & (~(DISTANCEREADINGBITMAP | AVEREADINGBITMAP));
            setChanged();
            notifyObservers();
        } else if (type == (DISTANCEREADINGBITMAP | MAXREADINGBITMAP)) {
            this.maxDistance = n & (~(DISTANCEREADINGBITMAP | MAXREADINGBITMAP));
            setChanged();
            notifyObservers();
        } else if (type == (DISTANCEREADINGBITMAP | MINREADINGBITMAP)) {
            this.minDistance = n & (~(DISTANCEREADINGBITMAP | MINREADINGBITMAP));
            setChanged();
            notifyObservers();
        } else {
            // Ignore it...  It's not valid.
        }
    }

    public int getValue() {
        return currentDistance;
    }

    @Override
    public int getcurrentDistanceReading() {
        return currentDistance;
    }

    @Override
    public int getMaxDistanceReading() {
        return maxDistance;
    }

    @Override
    public void run() {
        while (keepGoing) {
            // Read from the network.
            int msg = rnm.obtainNextMessage(1);
            setValue(msg);
        }
    }

    @Override
    public int getMinDistanceReading() {
        return minDistance;
    }

    @Override
    public int getAveDistanceReading() {
        return aveDistance;
    }
}
