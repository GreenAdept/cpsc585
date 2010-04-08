
#include "VictoryCalculator.h"
#include "MessageManager.h"

using namespace std;

VictoryCalculator::VictoryCalculator() {
	gameFinished = false;
	recorded = false;
	hasTimeLimit = false;
}

//------------------------------------------------------
// Function: calculateRank
// Calculates the ranking and remaining laps for the
// Vehicle in the provided vector/list with the specified
// index.
//------------------------------------------------------

void VictoryCalculator::calculateRank (vector<Vehicle*>& vehicles, int index) {
	Vehicle*   v = vehicles[index];
	VehicleAI* myAI = (VehicleAI*) v->getAI();
	int        rank = 1;

	if (myAI->isFinished()) {
		for (int i=0; i<finished.size(); i++)
			if (finished[i])
				rank++;

		finished[index] = true;
		ranks[index] = rank;
	}
	else {
		int passedWPs = myAI->getNumberofPassedWPs();
		float dist = myAI->getDistanceToNextWP (v->getPosition());

		for (int i=0; i<vehicles.size(); i++) {
			if (i == index) continue;
			VehicleAI* enemyAI = (VehicleAI*) vehicles[i]->getAI();

			if (passedWPs < enemyAI->getNumberofPassedWPs())
				rank++;
			if (passedWPs == enemyAI->getNumberofPassedWPs()) {
				if (dist > enemyAI->getDistanceToNextWP (vehicles[i]->getPosition()))
					rank++;
			}
		}

		ranks[index] = rank;
		v->setRank(rank);
	}
}

//------------------------------------------------------
// Function: calculateRanks
// Calculates the rankings and remaining laps for the
// Vehicles in the provided vector/list.
//------------------------------------------------------

void VictoryCalculator::calculateRanks (vector<Vehicle*> vehicles, int numPlayers) {
	if (ranks.size() != vehicles.size()) {
		ranks.resize (vehicles.size(), 1);
		finished.resize (vehicles.size(), false);
		finishTimes.resize (vehicles.size(), "00:00:00");
	}

	gameFinished = true;
	for (int i=0; i<vehicles.size(); i++) {
		if (!finished[i]) {
			calculateRank (vehicles, i);
			if (i < numPlayers)
				gameFinished = false;
		}
		else if (hasTimeLimit) { //if the vehicle finished, and there is a time limit
			if (finishTimes[i].compare(timeLimit) > 0) //if finishedTime is greater than timeLimit
			{	//set completed course to false
			}
			else
			{
				//set completed course to true, and record time. Decrease time by 30sec
			}
		}
	}

	if (gameFinished)
		Emit (Events::EGameFinished);
}


//------------------------------------------------------
// Function: getRank
// Returns the ranking of the vehicle (player or
// computer-controlled) with the specified index.
//
// Player 1 has index 0, Player 2 has index 1, etc...
//------------------------------------------------------

int VictoryCalculator::getRank (int index) {
	if (index >= ranks.size())
		return 0;
	else
		return ranks[index];
}

vector<int> VictoryCalculator::getRanks () {
	return ranks;
}

//------------------------------------------------------
// Function: isFinished
// Returns true if the vehicle (player or computer-
// controlled) with the specified index has finished
// the race, false otherwise.
//
// Player 1 has index 0, Player 2 has index 1, etc...
//------------------------------------------------------

bool VictoryCalculator::isFinished (int index) {
	if (index >= finished.size())
		return false;
	else
		return finished[index];
}

/*
wstring VictoryCalculator::getFormattedString (int index) {
	if (index >= finished.size())
		return L"%i";
	
	if (finished[index])
		return L"You finished in %i place";
	else
		return L"You are in %i place";
}

*/
void VictoryCalculator::recordTime(string time) {
	//if the player is not finished, or we have already recorded the time, return
	if (!isFinished(0)) return;
	if (recorded) return;

	times.clear();
	int numberOfRecordedTimes = 5;

	//get the current recordings from file
	ifstream fin("times.txt");
	while (!fin.eof()) {
		string s;
		fin >> s; //read the time from file
		if (s != "")
			times.push_back(s);
	}
	fin.close();

	if (times.size() < 5)
		for (int i = 0; i < 5-times.size(); i++)
			times.push_back("59:59:99");

	//insert the new times, and all but one of the old times
	bool inserted = false;
	vector<string>::iterator it;
	for (it = times.begin(); it < times.end(); it++){
		if (time.compare(*it) < 0) {
			times.insert(it, time);
			inserted = true;
			break;
		}
	}

	if (!inserted && (times.size() < numberOfRecordedTimes))
		times.push_back(time);

	ofstream fout("times.txt");
	for (int i = 0; i < 5; i++) {
		fout << times[i] << endl;
	}
	if (times.size() < 5)
		for (int i = 0; i < 5-times.size(); i++)
			fout << "59:59:99" << endl;

	fout.close();

	recorded = true;
}

vector<string> VictoryCalculator::getRecordedTimes()
{
	return times;
}


bool VictoryCalculator::isGameFinished () {
	return gameFinished;
}


//--------------------------------------------------------------------------------------
// Function: setFinishTime
// This function sets the finish time for a player.  
//--------------------------------------------------------------------------------------
void VictoryCalculator::setFinishTime (int playerNum, string time) {
	if( playerNum < 0 || playerNum > 7 )
		return;
	finishTimes [playerNum] = time;
}

//--------------------------------------------------------------------------------------
// Function: getFinishTimes
//--------------------------------------------------------------------------------------
vector<string> VictoryCalculator::getFinishTimes () {
	return finishTimes;
}

void VictoryCalculator::setTimeLimit(string limit)
{
	hasTimeLimit = true;
	timeLimit = limit;
}

string VictoryCalculator::getTimeLimit( )
{
	if (hasTimeLimit)
		return timeLimit;
	return NULL;
}