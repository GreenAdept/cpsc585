#include "TriggerVolumeAI.h"
#include "EntityManager.h"

TriggerCylinder::TriggerCylinder(Vec3 position, float radius) {
	this->position = position;
	this->radius = radius*radius;
}

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

void TriggerCylinder::think (EntityManager *em) {
	if (state == AI::TRIGGERED)
		state = AI::STOPPED;

	if (state == AI::STOPPED)
		return;

	std::vector<Vehicle*> vehicles = em->getVehicles();

	for (int i = 0; i < vehicles.size(); i++) {
		Vec3 vehicle = vehicles[i]->getPosition();
		
		float d = distSquared (vehicle, position);
		if (d <= radius) {
			state = AI::TRIGGERED;
			break;
		}
	}

	if (state != AI::TRIGGERED)
		state = AI::WAITING;
}
