/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"


//--------------------------------------------------------------------------------------
// Function: think
// A stub for the thinking section of the AI.
//--------------------------------------------------------------------------------------

void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;
	if (destination == 0)
		destination = em->getTerrain()->getTrackStart();

	Vec3 myPos = em->getPosition (myList, myIndex);
	Vec3 destPos = destination->getPosition ();
	if (distSquared (myPos, destPos) < 25)
		state = AI::TRIGGERED;

	Input* input = em->getComputerInputObj (myIndex);
	input->setInput (Input::UP, true);
}
