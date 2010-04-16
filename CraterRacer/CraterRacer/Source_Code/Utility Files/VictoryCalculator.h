#ifndef VICTORY_H
#define VICTORY_H

#include "Constants.h"
#include "Vehicle.h"
#include "Clock.h"
#include <vector>
#include <string>

//-----------------------------------------
// Class: VictoryCalculator
//-----------------------------------------

class VictoryCalculator {
	vector<int>    ranks;
	vector<bool>   finished;
	vector<string> finishTimes;
	vector<string> times;
	bool           gameFinished;
	bool		   recorded;
	bool		   hasTimeLimit;
	string		   timeLimit;

	void		   calculateRank  (std::vector<Vehicle*>& vehicles, int index);

public:
	VictoryCalculator();

	void           calculateRanks	(std::vector<Vehicle*> vehicles, int numberOfPlayers);
	int            getRank			(int index);
	vector<int>    getRanks			( );
	bool           isFinished		(int index);
	bool	       isGameFinished	( );
	void	       setFinishTime	(int playerNum, string time);
	vector<string> getFinishTimes	(std::vector<Vehicle*> vehicles);
	void		   recordTime		(string time);
	vector<string> getRecordedTimes	( );
	void		   setTimeLimit		( string limit );
	string		   getTimeLimit		( );
};


#endif
