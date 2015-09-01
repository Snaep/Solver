#include "stopwatch.h"


void Stopwatch_Start( struct Stopwatch* timer ) {
	//determinate performance frequency and store as double converted value
#ifdef STOPWATCH_MODE_SECONDS
	LARGE_INTEGER fq;
	QueryPerformanceFrequency( &fq );
	timer->fq = ( double )fq.QuadPart;
#endif
	//create reference point
	QueryPerformanceCounter( &timer->li );
}

#ifdef STOPWATCH_MODE_SECONDS
double Stopwatch_GetTime( struct Stopwatch* timer ) {
	//get number of performance counter-ticks
	QueryPerformanceCounter( &timer->si );

	//calculate time passed since first init
	return ( timer->si.QuadPart - timer->li.QuadPart ) / timer->fq;
}
#else
unsigned long long Stopwatch_GetTime( struct Stopwatch* timer ) {
	//get number of performance counter-ticks
	QueryPerformanceCounter( &timer->si );
	//calculate time passed since first init
	return ( timer->si.QuadPart - timer->li.QuadPart );
}
#endif

