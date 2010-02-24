#ifndef AI_H
#define AI_H

#include "Constants.h"
class EntityManager;

//---------------------------------------------
// Class: AI
// AI abstract base class, from which concrete
// AI classes will be derived.
//---------------------------------------------

class AI {
protected:
	int state;

public:
	enum {WAITING, STOPPED, MOVING, TRIGGERED};
	int getState () { return state; }
	void setState (int stateNew) { state = stateNew; }

	AI () { state = WAITING; }

	virtual void think (EntityManager *em, int myList, int myIndex) = 0;
};



//---------------------------------------------
// Function: distSquared
// Returns the square of the distance (as the
// crow flies) between two points.
//---------------------------------------------

float distSquared (Vec3 A, Vec3 B);

#endif