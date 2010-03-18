/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"
#include <math.h>


//------------------------------------------------------
// Function: think
// Checks if the vehicle has reached its destination
// waypoint, and updates the destination waypoint and
// lap count.
//------------------------------------------------------

void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;

	Vehicle* myEntity = (Vehicle*) (*em)[myList][myIndex];
	Vec3     myPos = myEntity->getPosition ();

	if (state == AI::WAITING) {
		Terrain* terrain = em->getTerrain();
		if (terrain == 0) return;

		laps = terrain->getNumberOfLaps();
		destination = terrain->getTrackStart();
		if (destination == 0) return;

		lastPassedWaypoint = myPos;
		state = AI::MOVING;
	}

  //Check if the destination has been reached
	Vec3 destPos = destination->getPosition();
	if (distSquared (myPos, destPos) < 1600.0f) {
		lastPassedWaypoint = destPos;
		passedWPs++;
		destination = destination->getRandomNext();

		if (destination == 0) {
			laps--;
			if (laps == 0)
				state = AI::STOPPED;
			else
				destination = em->getTerrain()->getTrackStart();
		}
	}


	if (myList == PLAYERS && destination != 0) {
		Vec3 currentDir = myEntity->getDirection ();
		Vec3 desiredDir = destination->getDirectionToWP (myPos);

		float cosTheta = D3DXVec3Dot (&currentDir, &desiredDir);
		if (cosTheta < 0)
			wrongWay = true;
		else
			wrongWay = false;
	}
}

Vec3 VehicleAI::getNextWaypoint() {
	if (destination != 0)
		return destination->getPosition();
	else
		return Vec3(0, 0, 0);
}

float VehicleAI::getDistanceToNextWP (Vec3 myPos) {
	if (destination == 0)
		return 0;
	else
		return sqrt (distSquared (myPos, destination->getPosition()));
}




//------------------------------------------------------
// Function: think
// Sets the input for the AI vehicle based on its
// desired destination.
//------------------------------------------------------

void CompVehicleAI::think (EntityManager *em, int myList, int myIndex) {
	VehicleAI::think (em, myList, myIndex);
	if (state == AI::STOPPED || state == AI::WAITING) return;

	Vehicle* myEntity = (Vehicle*) (*em)[myList][myIndex];
	Vec3     myPos = myEntity->getPosition();
	Input*   input = myEntity->getInputObj();

  //Find the current direction of travel
	Vec3 currentDir = myEntity->getDirection ();

	vector<Entity*> obstacles = em->getObstacles();
	for (int i=0; i<obstacles.size(); i++) {
		Vec3 obstacle = obstacles[i]->getPosition();
		if (distSquared (myPos, obstacle) < 400.0f) {
			Vec3 dirOfObstacle = obstacle - myPos;
			D3DXVec3Normalize (&dirOfObstacle, &dirOfObstacle);
			if (avoid (currentDir, dirOfObstacle, input))
				return;
		}
	}

  //Find the desired direction of travel and steer there
	Vec3 desiredDir = destination->getDirectionToWP (myPos);
	steer (currentDir, desiredDir, input);
}

//------------------------------------------------------
// Function: steer
// Sets the input for the AI vehicle based on the
// difference between its current and desired directions.
//------------------------------------------------------

void CompVehicleAI::steer (Vec3& currentDir, Vec3& desiredDir, Input* input) {
  //Find the sine of the angle between the two directions
	Vec3 temp;
	D3DXVec3Cross (&temp, &currentDir, &desiredDir);
	float cosTheta = D3DXVec3Dot (&currentDir, &desiredDir);
	float sinTheta = D3DXVec3Length (&temp);

  //Set input based on angle
	if (cosTheta >= 0.0f) {
		input->setKey (Input::D_KEY, true);
		if (sinTheta > 0.2f) {
			if (temp.y > 0.0) input->setInput (Input::RIGHT, true);
			else              input->setInput (Input::LEFT, true);
		}
	}
	else {
		input->setKey (Input::S_KEY, true);
		if (temp.y > 0.0) input->setInput (Input::LEFT, true);
		else              input->setInput (Input::RIGHT, true);
	}
	input->setPressure(1.0f);
}

//------------------------------------------------------
// Function: avoid
// Sets the input for the AI vehicle to avoid an obstacle
// based on the difference between its current direction
// and the direction of the obstacle. Returns true if the
// vehicle is steering to avoid the obstacle.
//------------------------------------------------------

bool CompVehicleAI::avoid (Vec3& currentDir, Vec3& dirOfObstacle, Input* input) {
  //Find the sine of the angle between the two directions
	Vec3 temp;
	D3DXVec3Cross (&temp, &currentDir, &dirOfObstacle);
	float cosTheta = D3DXVec3Dot (&currentDir, &dirOfObstacle);

  //Set input based on angle
	if (cosTheta >= 0.707f) {
		input->setKey (Input::D_KEY, true);
		if (temp.y < 0.0) input->setInput (Input::RIGHT, true);
		else              input->setInput (Input::LEFT, true);
		input->setPressure(1.0f);
		return true;
	}
	else
		return false;
}