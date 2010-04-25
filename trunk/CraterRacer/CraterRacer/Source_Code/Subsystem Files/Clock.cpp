#include "Clock.h"
#include "MessageManager.h"

Clock::Clock()
{
	startTimeMS = 0;
	lastTimeMS = 0;

	totalTimeMS = 0;
	totalPausedTimeMS = 0;
	startPausedMS = 0;
	started = false;
	stopped = true;
}

//--------------------------------------------------------------------------------------
// function: start
// Sets all time counters to 0, and sets start time to now. Pause is set to false.
//--------------------------------------------------------------------------------------
void Clock::start()
{
	paused = false;
	startTimeMS = GetTickCount();
	lastTimeMS = startTimeMS;

	totalTimeMS = 0;
	totalPausedTimeMS = 0;
	startPausedMS = 0;
	started = true;
	stopped = false;
}

void Clock::stop()
{
	stopped = true;
	started = true;
}

int Clock::getStartTimeInMS()
{
	return startTimeMS;
}

//--------------------------------------------------------------------------------------
// function: isPaused
// Returns true if clock is currently paused, else returns false.
//--------------------------------------------------------------------------------------
bool Clock::isPaused()//returns true if clock is currently paused
{
	return paused;
}

//--------------------------------------------------------------------------------------
// function: togglePause
// Toggles between paused and not paused status, and returns the value of paused.
//--------------------------------------------------------------------------------------
bool Clock::togglePause() //change from paused to resume, and vice versa
{
	paused = !paused;
	if (paused)
		//getTotalTimeInMS(); //calculate elapsedTime
		startPausedMS = GetTickCount();
	else {
		//startTimeMS = GetTickCount(); //reset the start time when resuming
		int time = GetTickCount();
		totalPausedTimeMS = totalPausedTimeMS + time - startPausedMS;
	}
	return paused;
}

//--------------------------------------------------------------------------------------
// function: getTotalTimeInMS
// Returns the total time since calling start() in milliseconds.
//--------------------------------------------------------------------------------------
int Clock::getTotalTimeInMS() //returns the elapsed time in milliseconds
{
	if (stopped)
		return 0;
	long time = GetTickCount(); //get current time

	if (paused)
	{
		// startPausedMS - startTimeMS - totalPausedTimeMS
		totalTimeMS = (startPausedMS - startTimeMS) - totalPausedTimeMS;
	}
	else
	{
		// total time = last elapsed time + time that passed since then
		totalTimeMS = (time - startTimeMS) - totalPausedTimeMS; 
	}
	return totalTimeMS;
}

//--------------------------------------------------------------------------------------
// function: getTotalTimeInSec
// Returns the total time since calling start() in seconds.
//--------------------------------------------------------------------------------------
int Clock::getTotalTimeInSec() //returns the elapsed time in seconds
{
	return (int)(getTotalTimeInMS()/1000);
}

//--------------------------------------------------------------------------------------
// function: getElapsedTimeInMS
// Returns the time elapsed since last calling getElapsedTime...() in milliseconds.
//--------------------------------------------------------------------------------------
int Clock::getElapsedTimeInMS()
{
	if (stopped)
		return 0;
	int nowTime = getTotalTimeInMS();
	//elapsed time is total time now - last time retrieved
	int elapsedTimeMS = nowTime - lastTimeMS; 
	lastTimeMS = nowTime;
	return elapsedTimeMS;
}

//--------------------------------------------------------------------------------------
// function: getElapsedTimeInSec
// Returns the time elapsed since last calling getElapsedTime...() in seconds.
//--------------------------------------------------------------------------------------
int Clock::getElapsedTimeInSec()
{
	return (int)(getElapsedTimeInMS()/1000);
}

//--------------------------------------------------------------------------------------
// function: getFormattedTime
// Returns a string of the current time in the format: minutes:seconds:ten_milliseconds.
// ten_milliseconds is milliseconds/10.
//--------------------------------------------------------------------------------------
string Clock::getFormattedTime() {
	return getFormattedTime(getTotalTimeInMS());
}

string Clock::getFormattedTime(int time) //in milliseconds
{
	string s;
	stringstream ss;

	if (time >= 3600000)
		s = "59:59:99";
	else if (stopped)
		s = "00:00:00";
	else {
		int minutes = time/60000;
		int remaining = time - (minutes * 60000); //number of ms left
		int seconds = remaining/1000;
		remaining = remaining - (seconds * 1000); //number of ms left
		int ten_milliseconds = remaining/10;
		
		ss << ten_milliseconds;
		string ms = ss.str();
		ss.str("");
		if (ten_milliseconds < 10)
			ms = "0" + ms;

		ss << seconds;
		string sec = ss.str();
		ss.str("");
		if (seconds < 10)
			sec = "0" + sec;

		ss << minutes;
		string mins = ss.str();
		ss.str("");
		if (minutes < 10)
			mins = "0" + mins;

		//ss << minutes << ":" << seconds << ":" << ten_milliseconds;
		//s = ss.str();
		ss << mins << ":" << sec << ":" << ms;
		s = ss.str();
	}

	return s;
}
