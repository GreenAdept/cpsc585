
#include "VictoryCalculator.h"
using namespace std;

VictoryCalculator::VictoryCalculator() {
	recorded = false;
}

//------------------------------------------------------
// Function: calculateRank
// Calculates the ranking and remaining laps for the
// Vehicle in the provided vector/list with the specified
// index.
//------------------------------------------------------

void VictoryCalculator::calculateRank (vector<Vehicle*>& vehicles, int index) {
	if (finished[index]) return;

	Vehicle* v = vehicles[index];
	VehicleAI* myAI = (VehicleAI*) v->getAI();
	int rank = 1;

	if (myAI->finished()) {
		for (int i=0; i<finished.size(); i++)
			if (finished[i])
				rank++;

		finished[index] = true;
		laps[index] = 0;
		ranks[index] = rank;
	}
	else {
		int passedWPs = myAI->getNumberofPassedWaypoints();
		float dist = myAI->getDistanceToNextWP (v->getPosition());

		for (int i=0; i<vehicles.size(); i++) {
			if (i == index) continue;
			VehicleAI* enemyAI = (VehicleAI*) vehicles[i]->getAI();

			if (passedWPs < enemyAI->getNumberofPassedWaypoints())
				rank++;
			if (passedWPs == enemyAI->getNumberofPassedWaypoints()) {
				if (dist > enemyAI->getDistanceToNextWP (vehicles[i]->getPosition()))
					rank++;
			}
		}

		laps[index] = myAI->getRemainingLaps();
		ranks[index] = rank;
	}
}

//------------------------------------------------------
// Function: calculateRanks
// Calculates the rankings and remaining laps for the
// Vehicles in the provided vector/list.
//------------------------------------------------------

void VictoryCalculator::calculateRanks (vector<Vehicle*> vehicles) {
	if (ranks.size() != vehicles.size()) {
		ranks.resize (vehicles.size(), 1);
		laps.resize (vehicles.size(), 1);
		finished.resize (vehicles.size(), false);
	}

	for (int i=0; i<vehicles.size(); i++)
		calculateRank (vehicles, i);
}

//------------------------------------------------------
// Function: getRank
// Returns the number of laps remaining for the vehicle
// (player or computer-controlled) with the specified
// index.
//
// Player 1 has index 0, Player 2 has index 1, etc...
//------------------------------------------------------

int VictoryCalculator::getRemainingLaps (int index) {
	if (index >= laps.size())
		return -1;
	else
		return laps[index];
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

//------------------------------------------------------
// Function: isFinished
// Returns true if the vehicle (player or computer-
// controlled)with the specified index has finished
// the race, false otherwise.
//
// Player 1 has index 0, Player 2 has index 1, etc...
//------------------------------------------------------

bool VictoryCalculator::isFinished (int index) {
	if (index >= ranks.size())
		return false;
	else
		return finished[index];
}

wstring VictoryCalculator::getFormattedString (int index) {
	if (index >= finished.size())
		return L"%i";
	
	if (finished[index])
		return L"You finished in %i place";
	else
		return L"You are in %i place";
}


void VictoryCalculator::recordTime(int milliseconds) {
	//if the player is not finished, or we have already recorded the time, return
	if (!finished[0]) return;
	if (recorded) return;

	int numberOfRecordedTimes = 5;
	int *times = new int[numberOfRecordedTimes];

	//get the current recordings from file
	ifstream fin("times.txt");
	for (int i = 0; i < numberOfRecordedTimes; i++) {
		if (!fin.eof()) {
			fin >> times[i]; //read the time from file
		}
		else {
			times[i] = 500000; //a really big constant
		}
	}
	fin.close();

	ofstream fout("times.txt");

	//insert the new times, and all but one of the old times
	bool inserted = false;
	for (int i = 0; i < numberOfRecordedTimes-1; i++) {
		if (milliseconds < times[i] && !inserted) {
			fout << milliseconds << endl;;
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

// This function returns true when at least one player is finished
bool VictoryCalculator::isGameFinished( )
{
	return finished[ PLAYER1 ];
}