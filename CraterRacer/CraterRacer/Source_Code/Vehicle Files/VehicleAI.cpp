/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"


//--------------------------------------------------------------------------------------
// Function: think
// A stub for the thinking section of the AI.
//--------------------------------------------------------------------------------------

//#include "DebugWriter.h"
//DebugWriter db;

void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;
	if (destination == 0)
		destination = em->getTerrain()->getTrackStart();

	Entity* myEntity = (*em)[myList][myIndex];

  //Check if the destination has been reached
	Vec3 myPos = myEntity->getPosition();
	Vec3 destPos = destination->getPosition();
	if (distSquared (myPos, destPos) < 100.0f) {
		destination = destination->getRandomNext();
		if (destination == 0)
			state = AI::STOPPED;
	}

  //Find the direction of travel and the desired direction of travel
	Vec3 currentDir (0, 0, 1);
	D3DXVec3TransformNormal (&currentDir, &currentDir, &myEntity->getPositionMatrix());
	Vec3 desiredDir = destPos - myPos;
	D3DXVec3Normalize (&desiredDir, &desiredDir);

  //Find the sine of the angle between the two directions
	Vec3 temp;
	D3DXVec3Cross (&temp, &currentDir, &desiredDir);
	float cosTheta = D3DXVec3Dot (&currentDir, &desiredDir);
	float sinTheta = D3DXVec3Length (&temp);

  //Set input based on angle
	Input* input = em->getComputerInputObj (myIndex);
	input->setInput (Input::UP, true);
	//Turn if angle > 30 degrees
	if (sinTheta > 0.5f) {
		if (temp.y > 0.0) input->setInput (Input::RIGHT, true);
		else              input->setInput (Input::LEFT, true);
	}
}
