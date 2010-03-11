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
	std::vector<int>  ranks;
	bool			  recorded;

	void    calculateRank      (std::vector<Vehicle*>& vehicles, int index);

public:
	VictoryCalculator();
	void    calculateRanks     (std::vector<Vehicle*> vehicles);
	int     getRank            (int index);
	bool    isFinished         (int index);
	wstring getFormattedString (int index);
	void	recordTime		   (int milliseconds);
};


#endif
