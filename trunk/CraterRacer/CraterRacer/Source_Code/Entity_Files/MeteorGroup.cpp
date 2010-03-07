#include "MeteorGroup.h"
#include "EntityManager.h"

MeteorGroupAI::MeteorGroupAI (float radius) {
	trigger = new TriggerCylinder (radius);
}
MeteorGroupAI::~MeteorGroupAI () {
	delete trigger;
}

void MeteorGroupAI::think(EntityManager *em, int myList, int myIndex) {
	MeteorGroup* meteorGroup = (MeteorGroup*) (*em)[myList][myIndex];

	if (state == AI::STOPPED)
		return;
	if (state == AI::TRIGGERED) {
		state = AI::MOVING;
		return;
	}
	if (state == AI::MOVING) {
		for (int i=0; i<meteorGroup->numMeteors; i++)
			if (meteorGroup->meteors[i]->getAI()->getState() != AI::STOPPED)
				return;
		state = AI::STOPPED;
		return;
	}
	if (state == AI::WAITING) {
		trigger->think (em, myList, myIndex);
		state = trigger->getState();

		if (state == AI::TRIGGERED) {
			for (int i=0; i < meteorGroup->numMeteors; i++)
				meteorGroup->meteors[i]->informOfTrigger();
		}
		return;
	}
}






MeteorGroup::MeteorGroup (int id, int numMeteors, float radius) {
	this->id = id;
	this->numMeteors = numMeteors;
	meteors = new Meteor* [numMeteors];

	mind = new MeteorGroupAI (radius);
}
MeteorGroup::~MeteorGroup () {
	delete[] meteors;
	delete mind;
}

AI* MeteorGroup::getAI() {
	return mind;
}

void MeteorGroup::addMeteor (int index, Meteor* m) {
	meteors[index] = m;
}