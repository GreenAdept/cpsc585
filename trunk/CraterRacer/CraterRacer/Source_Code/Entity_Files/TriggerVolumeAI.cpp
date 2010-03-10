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

	vector<Vehicle*> vehicles = em->getVehicles ();
	Vec3 center = em->getPosition (myList, myIndex);
	
	for (int i=0; i<vehicles.size(); i++) {
		Vec3 pos = vehicles[i]->getPosition();
		float d = distSquared (pos, center);
		if (d <= radius) {
			state = AI::TRIGGERED;
			return;
		}
	}
	state = AI::WAITING;
}
