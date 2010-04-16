#ifndef VEHICLE_AI_H
#define VEHICLE_AI_H

#include "AI.h"
#include "Constants.h"

class Input;
class AIPath;

//-----------------------------------------
// Class: VehicleAI
// Implementation of a vehicle's AI, which
// tracks the vehicle's progress on the
// racetrack.
//-----------------------------------------

class VehicleAI : public AI {
protected:
	AIPath*   path;
	int       passedWPs, passedWPsLB;   //upper bound and lower bounds on number of passed waypoints
	int       currentLap;
	int		  m_iPlayerNum;

	float     elapsed;
	bool	  canModify;

public:
	VehicleAI (bool canModify = false);

	virtual void think (EntityManager *em, int myList, int myIndex);
	Vec3         getLastPassedWaypoint (Vec3 myPos);
	Vec3		 getNextWaypoint       (Vec3 myPos);
	float        getDistanceToNextWP   (Vec3 myPos);
	int          getNumberofPassedWPs  () { return passedWPs; }
	int          getCurrentLapNumber   ();
	bool         isFinished            () { return state == AI::STOPPED; }
	void		 setPlayerNum          (int num) { m_iPlayerNum = num; }
	void		 incWP				   ();
	void         timeElapsed           (float seconds) { elapsed += seconds; }
};


//-----------------------------------------
// Class: VehicleAI
// Implementation of a computer-controlled
// vehicle's AI.
//-----------------------------------------

class CompVehicleAI : public VehicleAI {
	void steer (Vec3& currentDir, Vec3& desiredDir, Input* input);
	bool avoid (Vec3& currentDir, Vec3& dirOfObstacle, Input* input);

public:
	void think (EntityManager *em, int myList, int myIndex);
};


#endif
