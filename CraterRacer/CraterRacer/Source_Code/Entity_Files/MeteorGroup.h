#pragma once
#ifndef MeteorGroup_H
#define MeteorGroup_H

#include "constants.h"
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

public:
	MeteorGroupAI (float radius);
	~MeteorGroupAI ();

	void think (EntityManager *em, int myList, int myIndex);
};

//---------------------------------------------
// Class: MeteorGroup
//---------------------------------------------

class MeteorGroup: public Entity {
	MeteorGroupAI* mind;

public:
	MeteorGroup (int id, int numMeteors, float radius);
	~MeteorGroup ();

	AI* getAI ();
	void addMeteor (int index, Meteor* m);

	int id;
	int numMeteors;
	Meteor** meteors;
};
#endif