#ifndef AI_H
#define AI_H

#include "Constants.h"

//---------------------------------------------
// Class: AI
// AI abstract base class, from which concrete
// AI classes will be derived.
//---------------------------------------------

class AI {
public:
	virtual void think () = 0;
};



//---------------------------------------------
// Function: distSquared
// Returns the square of the distance (as the
// crow flies) between two points.
//---------------------------------------------

float distSquared (Vec3 A, Vec3 B);

#endif