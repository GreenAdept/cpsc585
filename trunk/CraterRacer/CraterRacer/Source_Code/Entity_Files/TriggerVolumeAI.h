#ifndef TRIGGER_VOLUME_AI_H
#define TRIGGER_VOLUME_AI_H

#include "AI.h"


//------------------------------------------------------
// Class: TriggerCylinder
// Represents a cylindrical trigger volume with infinite
// height.
//------------------------------------------------------

class TriggerCylinder : public AI {
private:
	float radius;
	Vec3 position;

public:
	TriggerCylinder       () {}
	TriggerCylinder       (Vec3 position, float radius);
	TriggerCylinder       (float r) { radius = r*r; }
	void setTriggerRadius (float r) { radius = r*r; }

	void think (EntityManager *em, int myList, int myIndex);
	void think (EntityManager *em);
};


#endif
