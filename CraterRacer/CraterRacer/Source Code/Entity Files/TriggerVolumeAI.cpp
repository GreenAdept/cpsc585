#include "TriggerVolumeAI.h"


TriggerCylinder::TriggerCylinder (float r) {
	radius = r*r;
}

void TriggerCylinder::think (EntityManager *em, int myList, int myIndex) {
	Vec3 player = em->getPosition (PLAYERS, 0);
	Vec3 center = em->getPosition (myList, myIndex);
	
	float d = distSquared (player, center);
	if (d <= radius)
		state = AI::TRIGGERED;
	else
		state = AI::WAITING;
}