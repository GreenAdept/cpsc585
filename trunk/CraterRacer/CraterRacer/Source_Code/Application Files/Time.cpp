#include "gametime.h"
#include <stdio.h>
#include <windows.h>


long long ticksPerMicrosec = 1;
long long getTime () {
	LARGE_INTEGER time;
	QueryPerformanceCounter (&time);
	return time.QuadPart / ticksPerMicrosec;
}



Clock::Clock () {
	LARGE_INTEGER ticksPerSecond;
    QueryPerformanceFrequency (&ticksPerSecond);
	ticksPerMicrosec = ticksPerSecond.QuadPart / 1000000;
    reset();
}

long Clock::getRealTime_Micros () {
    return real;
}
long Clock::getRealTime_Millis () {
    return real / 1000;
}
long Clock::getRealTime_Seconds () {
    return real / 1000000;
}

long Clock::getGameTime_Micros () {
    return game;
}
long Clock::getGameTime_Millis () {
    return game / 1000;
}
long Clock::getGameTime_Seconds () {
    return game / 1000000;
}

long Clock::getElapsed_Micros () {
    return elapsed;
}
long Clock::getElapsed_Millis () {
    return elapsed / 1000;
}
long Clock::getElapsed_Seconds () {
    return elapsed / 1000000;
}

void Clock::update () {
    elapsed = getTime() - start - real;
    real += elapsed;
    if (!paused)
        game += elapsed;
}
void Clock::reset () {
    start = getTime();
    elapsed = 0;
    real = 0;
    game = 0;
    paused = false;
}
void Clock::pause () {
    paused = true;
}
void Clock::unpause () {
    paused = false;
}