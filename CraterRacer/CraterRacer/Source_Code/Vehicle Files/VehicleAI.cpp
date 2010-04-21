/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"
#include "MessageManager.h"
#include <cmath>

VehicleAI::VehicleAI (bool canModify) {
	this->canModify = canModify;
	path = 0;
	currentLap = 1;
	passedWPs = 0;
	passedWPsLB;
}

//------------------------------------------------------
// Function: think
// Checks if the vehicle has reached its destination
// waypoint, and updates the destination waypoint and
// lap count.
//------------------------------------------------------

#define WAYPOINT_RADIUS 60

void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	if (state == AI::STOPPED) return;

	Vehicle* myEntity = (Vehicle*) (*em)[myList][myIndex];
	Vec3     myPos = myEntity->getPosition ();

	if (state == AI::WAITING) {
		Terrain* terrain = em->getTerrain( 0 );
		if (terrain == 0) return;

		path = terrain->getTrack();
		elapsed = 0.0f;
		state = AI::MOVING;
	}

  //Check if the destination has been reached
	if (path->reachedWaypoint (myPos, passedWPsLB-1, WAYPOINT_RADIUS)) {
		passedWPsLB--;
		while (path->reachedWaypoint (myPos, passedWPsLB-1, WAYPOINT_RADIUS)) {
			passedWPsLB--;
		}
		passedWPs = passedWPsLB;
		while (path->reachedWaypoint (myPos, passedWPs+1, WAYPOINT_RADIUS)) {
			passedWPs++;
		}
	}
	else if (path->reachedWaypoint (myPos, passedWPs+1, WAYPOINT_RADIUS)) {
		elapsed = 0.0f;
		passedWPs++;
		while (path->reachedWaypoint (myPos, passedWPs+1, WAYPOINT_RADIUS)) {
			passedWPs++;
		}
		passedWPsLB = passedWPs;
		while (path->reachedWaypoint (myPos, passedWPsLB-1, WAYPOINT_RADIUS)) {
			passedWPsLB--;
		}
	}

  //Modify count of remaining laps, if needed
	int newLap = path->findCurrentLap (passedWPs);
	if (newLap > path->getNumberOfLaps()) {
		state = AI::STOPPED;
		//if (m_iPlayerNum >= 0)
			Emit (Events::EPlayerFinished, m_iPlayerNum);
		return;
	}
	if (newLap > currentLap) {
		currentLap = newLap;
		if (m_iPlayerNum >= 0)
			Emit (Events::ELapFinished, m_iPlayerNum, newLap);
	}

	
	if (myList == PLAYERS) {
		Vec3 currentDir = myEntity->getDirection ();
		bool wrongway = path->goingWrongWay (myPos, currentDir, passedWPs);

		if (wrongway)
			Emit( Events::EWrongWay, m_iPlayerNum );
		else
			Emit( Events::EWrongWayCancel, m_iPlayerNum );
	}
}


Vec3 VehicleAI::getLastPassedWaypoint (Vec3 myPos) {
	if (path == 0)
		return Vec3 (0, 0, 0);
	else
		return path->getPositionOfWP (myPos, passedWPs);
}

Vec3 VehicleAI::getNextWaypoint (Vec3 myPos) {
	if (path == 0)
		return Vec3 (0, 0, 0);
	else
		return path->getPositionOfWP (myPos, passedWPs+1);
}

float VehicleAI::getDistanceToNextWP (Vec3 myPos) {
	if (path == 0)
		return 0;
	else
		return path->getDistanceToWP (myPos, passedWPs+1);
}

int VehicleAI::getCurrentLapNumber () {
	if (path == 0)
		return 1;
	else
		return path->findCurrentLap (passedWPs);
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
	Vec3 desiredDir = path->getDirectionToWP (myPos, passedWPs+1);
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
		input->setDir (Input::UP, true);
		if (sinTheta > 0.2f) {
			if (temp.y > 0.0) input->setInput (Input::RIGHT, true);
			else              input->setInput (Input::LEFT, true);
		}
	}
	else {
		input->setDir (Input::DOWN, true);
		if (temp.y > 0.0) input->setInput (Input::LEFT, true);
		else              input->setInput (Input::RIGHT, true);
	}

	//respawn if 7 seconds have passed since the last waypoint was passed
	if (elapsed > 7.0f) {
		elapsed = 0.0f;
		input->setKey (Input::D_KEY, true);
	}
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
	if (cosTheta >= 0.866f) {
		input->setInput (Input::UP, true);
		if (temp.y < 0.0) input->setInput (Input::RIGHT, true);
		else              input->setInput (Input::LEFT, true);
		return true;
	}
	else
		return false;
}

void VehicleAI::incWP() {
	if (canModify) {
		passedWPs++;
	}
}