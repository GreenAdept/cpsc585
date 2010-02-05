#ifndef TRIGGER_VOLUME_AI_H
#define TRIGGER_VOLUME_AI_H

#include "AI.h"


class TriggerCylinder : public AI {
	float radius;

public:
	TriggerCylinder       (float r) { radius = r*r; }
	void setTriggerRadius (float r) { radius = r*r; }

	void think (EntityManager *em, int myList, int myIndex);
};


#endif
