/**
 * This is a set of time utilities.  It is used for basic timestamping.
 * @return
 */
#include <sys/time.h>

/**
 * This method will return a current timestamp.
 * @return The return will be a constantly incrementing timestamp of accuracy to the nearest ms.
 */
int current_timestamp();

/**
 * This method will calculate the time delta between two timestamps.
 * @param start This is the starting time.
 * @param end This is the ending time.
 * @return The return will be the delta time between these, in ms.
 */
double calculateDelta(struct timeval start, struct timeval end);
