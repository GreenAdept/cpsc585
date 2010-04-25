#pragma once
#ifndef MeteorGroup_H
#define MeteorGroup_H

#include "Constants.h"
#include "Entity.h"
#include "AI.h"
#include "TriggerVolumeAI.h"
#include "Meteor.h"
#include "DebugWriter.h"

//---------------------------------------------
// Class: MeteorGroupAI
//---------------------------------------------

class MeteorGroupAI : public AI {
	TriggerCylinder* trigger;
	int spawnProbability;

public:
	MeteorGroupAI (float radius, int spawnOnLap);
	~MeteorGroupAI ();

	void think (EntityManager *em, int myList, int myIndex);
	void setSpawnProbability (int sp);
};

//---------------------------------------------
// Class: MeteorGroup
//---------------------------------------------

class MeteorGroup: public Entity {
	MeteorGroupAI* mind;

public:
	MeteorGroup (int id, int numMeteors, float radius, int spawnOnLap);
	~MeteorGroup ();

	AI* getAI ();
	void addMeteor (int index, Meteor* m);
	void setMeteorSpawnProbability (int sp) { mind->setSpawnProbability (sp); }

	int id;
	int numMeteors;
	Meteor** meteors;
};
#endif