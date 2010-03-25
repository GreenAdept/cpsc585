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
	std::vector<int>    ranks;
	std::vector<bool>   finished;
	std::vector<string> finishTimes;
	bool                gameFinished;

	void           calculateRank  (std::vector<Vehicle*>& vehicles, int index);

public:
	VictoryCalculator();

	void           calculateRanks (std::vector<Vehicle*> vehicles, int numberOfPlayers);
	int            getRank        (int index);
	vector<int>    getRanks       ();
	bool           isFinished     (int index);
	bool	       isGameFinished ();
	void	       setFinishTime  (int playerNum, string time);
	vector<string> getFinishTimes ();
};


#endif
