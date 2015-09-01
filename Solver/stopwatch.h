#pragma once

#include "defaults.h"

//#define STOPWATCH_TIME

struct Stopwatch {
	LARGE_INTEGER si, li;
	double fq;
};

void Stopwatch_Start( struct Stopwatch* timer );

#ifdef STOPWATCH_MODE_SECONDS
double Stopwatch_GetTime( struct Stopwatch* timer );
#else
unsigned long long Stopwatch_GetTime( struct Stopwatch* timer );
#endif
