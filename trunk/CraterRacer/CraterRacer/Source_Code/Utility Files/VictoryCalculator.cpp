
#include "VictoryCalculator.h"
#include "MessageManager.h"

using namespace std;

VictoryCalculator::VictoryCalculator() {
	gameFinished = false;
	for (int i=0; i < 4; i++)
		finishTimes.push_back ("00:00:00");
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
	}

	gameFinished = true;
	for (int i=0; i<vehicles.size(); i++) {
		if (!finished[i]) {
			calculateRank (vehicles, i);
			if (i < numPlayers)
				gameFinished = false;
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


void VictoryCalculator::recordTime(string time) {
	//if the player is not finished, or we have already recorded the time, return
	if (!finished[0]) return;
	if (recorded) return;

	int numberOfRecordedTimes = 5;
	string *times = new string[numberOfRecordedTimes];

	//get the current recordings from file
	ifstream fin("times.txt");
	for (int i = 0; i < numberOfRecordedTimes; i++) {
		if (!fin.eof()) {
			fin >> times[i]; //read the time from file
		}
		else {
			times[i] = "59:59:99"; //a really big constant
		}
	}
	fin.close();

	ofstream fout("times.txt");

	//insert the new times, and all but one of the old times
	bool inserted = false;
	for (int i = 0; i < numberOfRecordedTimes-1; i++) {
		if ((time.compare(times[i]) < 0) && !inserted) {
			fout << time << endl;;
			inserted = true;
		}

		fout << times[i];
		if (i != numberOfRecordedTimes-2) {
			fout << endl;
		}
	}
	fout.close();
	delete[] times;

	recorded = true;
}
*/


bool VictoryCalculator::isGameFinished () {
	return gameFinished;
}


//--------------------------------------------------------------------------------------
// Function: setFinishTime
// This function sets the finish time for a player.  
//--------------------------------------------------------------------------------------
void VictoryCalculator::setFinishTime (int playerNum, string time) {
	if( playerNum < 0 || playerNum > 3 )
		return;
	finishTimes [playerNum] = time;
}

//--------------------------------------------------------------------------------------
// Function: getFinishTimes
//--------------------------------------------------------------------------------------
vector<string> VictoryCalculator::getFinishTimes () {
	return finishTimes;
}