#ifndef VEHICLE_AI_H
#define VEHICLE_AI_H

#include "AI.h"
#include "Constants.h"

class Input;
class Waypoint;

//-----------------------------------------
// Class: VehicleAI
// Implementation of a computer-controlled
// vehicle's AI.
//-----------------------------------------

class VehicleAI : public AI {
	Waypoint* destination;

	void steer (Vec3& currentDir, Vec3& desiredDir, Input* input);
	bool avoid (Vec3& currentDir, Vec3& dirOfObstacle, Input* input);

public:
	VehicleAI () { destination = 0; }
	void think (EntityManager *em, int myList, int myIndex);
};


#endif
