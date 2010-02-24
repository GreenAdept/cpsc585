#ifndef VEHICLE_AI_H
#define VEHICLE_AI_H

#include "AI.h"
#include "Constants.h"

class Waypoint;

//-----------------------------------------
// Class: VehicleAI
// Implementation of a computer-controlled
// vehicle's AI.
//-----------------------------------------

class VehicleAI : public AI {
	Waypoint* destination;

public:
	VehicleAI () { destination = 0; }
	void think (EntityManager *em, int myList, int myIndex);
};


#endif
