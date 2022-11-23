#include <chrono>
#include "time_util.h"

using namespace std::chrono;

int current_timestamp() {
	milliseconds end = duration_cast < milliseconds > (system_clock::now().time_since_epoch());
	return end.count();
}

/**
 * This method will calculate the time delta between two timestamps.
 * @param start This is the starting time.
 * @param end This is the ending time.
 * @return The return will be the delta time between these, in ms.
 */
double calculateDelta(struct timeval start, struct timeval end)
{
	double startms = start.tv_sec * 1000 + start.tv_usec / 1000.0;
	double endms = end.tv_sec * 1000 + end.tv_usec / 1000.0;

	return endms - startms;
}
