#ifndef TRIGGER_VOLUME_AI_H
#define TRIGGER_VOLUME_AI_H

#include "AI.h"


//------------------------------------------------------
// Class: TriggerCylinder
// Represents a cylindrical trigger volume with infinite
// height.
//------------------------------------------------------

class TriggerCylinder : public AI {
	float radius;
	int minLapToTrigger;  //can only be triggered by vehicles that have reached this lap

public:
	TriggerCylinder       (float r, int minLap = 0) { radius = r*r; minLapToTrigger = minLap; }
	void setTriggerRadius (float r) { radius = r*r; }

	void think (EntityManager *em, int myList, int myIndex);
};


#endif
