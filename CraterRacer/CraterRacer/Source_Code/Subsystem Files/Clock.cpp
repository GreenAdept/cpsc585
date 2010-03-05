#include "Clock.h"

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
	pausedTimeMS = 0;
	startPausedMS = 0;
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
		pausedTimeMS = time - startPausedMS;
		totalPausedTimeMS += pausedTimeMS;
	}
	return paused;
}

//--------------------------------------------------------------------------------------
// function: getTotalTimeInMS
// Returns the total time since calling start() in milliseconds.
//--------------------------------------------------------------------------------------
int Clock::getTotalTimeInMS() //returns the elapsed time in milliseconds
{
	long time = GetTickCount(); //get current time
	// total time = last elapsed time + time that passed since then
	totalTimeMS = (time - startTimeMS) - totalPausedTimeMS; 
	//startTimeMS = time; //start time resets
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
	int time = getTotalTimeInMS();
	//elapsed time is total time now - last time retrieved
	int elapsedTimeMS = elapsedTimeMS - lastTimeMS; 
	lastTimeMS = time;
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
string Clock::getFormattedTime()
{
	string s;
	stringstream ss;
	getTotalTimeInMS();

	int minutes = totalTimeMS/60000;
	int remaining = totalTimeMS - (minutes * 60000); //number of ms left
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
	return s;
}
