#include "TriggerVolumeAI.h"
#include "EntityManager.h"


//------------------------------------------------------
// Function: think
// Unless its state is set to STOPPED, think() sets the
// TriggerCylinder's state to TRIGGERED if the player
// is in the cylinder; WAITING otherwise.
//------------------------------------------------------

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
