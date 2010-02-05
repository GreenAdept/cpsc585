#include "TriggerVolumeAI.h"
#include "EntityManager.h"


void TriggerCylinder::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;

	Vec3 player = em->getPosition (PLAYERS, 0);
	Vec3 center = em->getPosition (myList, myIndex);
	
	float d = distSquared (player, center);
	if (d <= radius)
		state = AI::TRIGGERED;
	else
		state = AI::WAITING;
}