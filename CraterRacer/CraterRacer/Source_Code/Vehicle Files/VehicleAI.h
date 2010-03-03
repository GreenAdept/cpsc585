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
	int laps;

public:
	VehicleAI () { destination = 0; laps = 1; }
	virtual void think (EntityManager *em, int myList, int myIndex);
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
