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
	vector<string> names;
	bool           gameFinished;
	bool		   emittedGameFinished;
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
	vector<string> getFinishTimes	();
	int			   recordTime		(string time);
	void		   closeFile		(int index, string name);
	vector<string> getRecordedTimes	( );
	vector<string> getRecordedNames	( );
	void		   setTimeLimit		( string limit );
	string		   getTimeLimit		( );
};


#endif
