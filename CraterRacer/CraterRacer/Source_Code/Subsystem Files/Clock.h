#define  NOMINMAX
#include "Windows.h"
#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <string>
#include <sstream>

using namespace std;

class Clock{
public:
	void start(); //to start or reset the clock
	bool isPaused(); //returns true if clock is currently paused
	bool togglePause(); //change from paused to resume, and vice versa
	int getStartTimeInMS();
	int getTotalTimeInMS(); //returns the total time since calling start() or resume() in milliseconds
	int getTotalTimeInSec(); //returns the total time since calling start() or resume() in milliseconds
	int getElapsedTimeInMS(); //returns the elapsed time since last retrieved in milliseconds
	int getElapsedTimeInSec(); //returns the elapsed time since last retrieved in seconds
	string getFormattedTime(); //returns time as minutes:seconds:milliseconds

private:
	int startTimeMS;
	int totalTimeMS;
	int lastTimeMS; //last retrieved time
	int totalPausedTimeMS;
	int startPausedMS;
	bool paused;
};

#endif