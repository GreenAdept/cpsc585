#ifndef VEHICLE_AI_H
#define VEHICLE_AI_H

#include "AI.h"
#include "Constants.h"

class Input;
class Waypoint;

//-----------------------------------------
// Class: VehicleAI
// Implementation of a vehicle's AI, which
// tracks the vehicle's progress on the
// racetrack.
//-----------------------------------------

class VehicleAI : public AI {
protected:
	Waypoint* destination;
	Vec3      lastPassedWaypoint;
	int       laps, passedWPs;
	bool      wrongWay;
	int		  m_iPlayerNum;

public:
	VehicleAI () { destination = 0; laps = 2; passedWPs = 0; wrongWay = false; }

	virtual void think (EntityManager *em, int myList, int myIndex);
	Vec3         getLastPassedWaypoint () { return lastPassedWaypoint; }
	Vec3		 getNextWaypoint       (Vec3 myPos);
	float        getDistanceToNextWP   (Vec3 myPos);
	int          getNumberofPassedWPs  () { return passedWPs; }
	int          getRemainingLaps      () { return laps; }
	bool         isFinished            () { return laps == 0; }
	bool         isGoingWrongWay       () { return wrongWay; }
	void		 setPlayerNum( int num ) { m_iPlayerNum = num; }
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
