#ifndef TRIGGER_VOLUME_AI_H
#define TRIGGER_VOLUME_AI_H

#include "AI.h"
#include "EntityManager.h"


class TriggerCylinder : public AI {
	float radius;

public:
	TriggerCylinder (float radius);
	void think (EntityManager *em, int myList, int myIndex);
};


#endif
