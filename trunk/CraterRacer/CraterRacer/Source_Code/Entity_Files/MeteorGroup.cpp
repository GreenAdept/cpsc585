#include "MeteorGroup.h"
#include "EntityManager.h"

MeteorGroupAI::MeteorGroupAI (float radius, int spawnOnLap) {
	trigger = new TriggerCylinder (radius, spawnOnLap);
	spawnProbability = 50;
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
			for (int i=0; i < meteorGroup->numMeteors; i++) {
				if (rand() % 100 < spawnProbability)
					meteorGroup->meteors[i]->informOfTrigger();
			}
		}
		return;
	}
}

void MeteorGroupAI::setSpawnProbability (int sp) {
	if (sp >= 0 || sp <= 100)
		spawnProbability = sp;
}




MeteorGroup::MeteorGroup (int id, int numMeteors, float radius, int spawnOnLap) {
	this->id = id;
	this->numMeteors = numMeteors;
	meteors = new Meteor* [numMeteors];

	mind = new MeteorGroupAI (radius, spawnOnLap);
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