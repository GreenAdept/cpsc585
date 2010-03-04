#include "MeteorGroup.h"
#include "EntityManager.h"

MeteorGroupAI::MeteorGroupAI() {}

MeteorGroupAI::MeteorGroupAI(Vec3 position, int radius) {
	trigger = new TriggerCylinder(position, radius);
	state = AI::WAITING;
}

void MeteorGroupAI::think(EntityManager *em, int myList, int myIndex) {
	DebugWriter debug;

	MeteorGroup* meteorGroup = em->getMeteorGroups()[myIndex];
	
	//If the meteor group is waiting, test it's trigger volume for a collision
	if (state == AI::WAITING) {
		trigger->think(em);

		if (trigger->getState() == AI::TRIGGERED) {
			state = AI::TRIGGERED;
		}
	}

	//If the meteor group has been triggered, set it's state to moving
	else if (state == AI::TRIGGERED) {
		state = AI::MOVING;
	}

	//If the meteor group is moving, check to see if all it's meteors have crashed.
	//If they have, set the group to stopped
	if (state == AI::MOVING) {
		bool finished = true;

		for (int i = 0; i < meteorGroup->numMeteors; i++) {
			if (meteorGroup->meteors[i]->getAI()->getState() != AI::STOPPED) {
				finished = false;
				break;
			}
		}

		if (finished) {
			state = AI::STOPPED;
		}
	}

	//If the meteor group is set to stopped, it is finished - do nothing
	if (state == AI::STOPPED) {
		return;
	}

	
}






MeteorGroup::MeteorGroup() {}

MeteorGroup::MeteorGroup(int id, int numMeteors) {
	this->id = id;
	this->numMeteors = numMeteors;
	meteors = new Meteor*[numMeteors];
}

void MeteorGroup::setTriggerVolume(Vec3 position, int radius) {
	mind = new MeteorGroupAI(position, radius);
}

AI* MeteorGroup::getAI() {
	return mind;
}

void MeteorGroup::addMeteor(int index, Vec3 position) {
	meteors[index] = new Meteor(position);
}