#include <cstddef>
#include "time.h"

double TimeStampMicro() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double timeStamp;

    timeStamp = now.tv_sec * 1e6;
    timeStamp = timeStamp + now.tv_usec;
    return timeStamp;
}

double MicroToSecond(double micro) {
	return micro * 1e-6;
}
