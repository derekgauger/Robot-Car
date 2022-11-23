package edu.msoe.se3910.imageTransmit.core;
import java.util.LinkedList;

/**
 * This class will hold a list of numbers of up to a given size and then performs statistical calculations on those numbers.
 * @author wws
 *
 */
public class LongListWithStatistics {
	/**
	 * This is the data that is being stored internally and analyzed.
	 */
	private LinkedList<Long> data = new LinkedList<Long>();
	/**
	 * This is the max number of items that are to be kept.
	 */
	private int maxDataSetSize;
	/**
	 * This is a running sum of all of the values that are inside of the list.
	 */
	private long sum = 0;

	/**
	 * This constructor will instantiate a list that can hold up to N items.
	 * @param maxDataSetSize This is the maximum data set size.
	 */
	public LongListWithStatistics(int maxDataSetSize) {
		this.maxDataSetSize = maxDataSetSize;
	}

	/**
	 * This method will add an item to the list.
	 * @param value This si the value that is to be added.
	 */
	public void addDataItem(long value) {
		if (data.size() < maxDataSetSize) {
			// Simply add the number.
		} else {
			// We need to remove an old number first.
			long removedVal = data.removeFirst();
			sum -= removedVal;
		}
		data.addLast(value);
		sum += value;
	}

	/**
	 * This method will return the average value of the items.
	 * @return This is the average value that is to be returned.
	 */
	public double getAverage() {
		double retValue = 0.0;
		if (data.size() > 0) {
			retValue = ((double) sum) / data.size();
		}
		return retValue;
	}

	/**
	 * This method will obtain the variance of the values within the data set.
	 * @return The variance will be returned.
	 */
	public double getVariance() {
		double mean = getAverage();
		double variance = 0.0;

		if (data.size() > 0) {
			for (long x : data) {
				// Subtract the mean
				double delta = x - mean;
				// Square delta.
				delta *= delta;

				// Add it to the total variance.
				variance += delta;
			}
			variance = variance / data.size();
		}
		return variance;
	}

	/**
	 * This method will obtain the standard deviation of the values within the data set.
	 * @return The STD will be returned.
	 */
	public double getSTD() {
		double variance = getVariance();
		return Math.sqrt(variance);
	}

}
