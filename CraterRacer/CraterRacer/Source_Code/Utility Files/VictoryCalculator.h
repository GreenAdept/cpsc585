#ifndef VICTORY_H
#define VICTORY_H

#include "Constants.h"
#include "Vehicle.h"
#include <vector>
#include <string>

//-----------------------------------------
// Class: VictoryCalculator
//-----------------------------------------

class VictoryCalculator {
	std::vector<bool> finished;
	//std::vector<bool> wrongWay;
	std::vector<int>  ranks;
	std::vector<int>  laps;
	bool			  recorded;

	void    calculateRank      (std::vector<Vehicle*>& vehicles, int index);

public:
	VictoryCalculator();

	void    calculateRanks     (std::vector<Vehicle*> vehicles);
	int     getRank            (int index);
	int     getRemainingLaps   (int index);
	bool    isFinished         (int index);
	//bool    isGoingWrongWay    (int index);
	wstring getFormattedString (int index);
	void	recordTime		   (int milliseconds);
	bool	isGameFinished	   ( );
};


#endif
