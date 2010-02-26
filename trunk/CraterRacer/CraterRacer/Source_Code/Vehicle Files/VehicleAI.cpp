/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"


//--------------------------------------------------------------------------------------
// Function: think
// Sets the input for the AI vehicle based on its desired destination.
//--------------------------------------------------------------------------------------

void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;
	if (state == AI::WAITING) {
		Terrain* terrain = em->getTerrain();
		if (terrain == 0) return;

		destination = terrain->getTrackStart();
		if (destination == 0) return;

		state = AI::MOVING;
	}

	Vehicle* myEntity = (Vehicle*) (*em)[myList][myIndex];
	Vec3     myPos = myEntity->getPosition();
	Input*   input = myEntity->getInputObj();

  //Check if the destination has been reached
	Vec3 destPos = destination->getPosition();
	if (distSquared (myPos, destPos) < 400.0f) {
		destination = destination->getRandomNext();
		if (destination == 0) {
			state = AI::STOPPED;
			return;
		}
	}

  //Find the direction of travel and the desired direction of travel
	Vec3 currentDir = myEntity->getDirection ();
	Vec3 desiredDir = destination->getDirectionToWP (myPos);

  //Find the sine of the angle between the two directions
	Vec3 temp;
	D3DXVec3Cross (&temp, &currentDir, &desiredDir);
	float cosTheta = D3DXVec3Dot (&currentDir, &desiredDir);
	float sinTheta = D3DXVec3Length (&temp);

  //Set input based on angle
	if (cosTheta >= -0.707f) {
		input->setInput (Input::UP, true);
		//Turn if angle > 30 degrees
		if (sinTheta > 0.5f) {
			if (temp.y > 0.0) input->setInput (Input::RIGHT, true);
			else              input->setInput (Input::LEFT, true);
		}
	}
	else {
		input->getButtons()[Input::X_BUTTON] = true;
		if (temp.y > 0.0) input->setInput (Input::LEFT, true);
		else              input->setInput (Input::RIGHT, true);
	}
}
