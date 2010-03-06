
#include "VictoryCalculator.h"
using namespace std;


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

		ranks[index] = rank;
	}
}

void VictoryCalculator::calculateRanks (vector<Vehicle*> vehicles) {
	if (ranks.size() != vehicles.size()) {
		ranks.resize (vehicles.size(), 1);
		finished.resize (vehicles.size(), false);
	}

	for (int i=0; i<vehicles.size(); i++)
		calculateRank (vehicles, i);
}

int VictoryCalculator::getRank (int index) {
	if (index >= ranks.size())
		return 0;
	else
		return ranks[index];
}

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
