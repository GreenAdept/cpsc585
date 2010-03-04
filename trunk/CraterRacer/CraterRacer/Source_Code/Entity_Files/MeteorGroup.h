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
// Class: MeteorAI
//---------------------------------------------

class MeteorGroupAI : public AI {
	TriggerCylinder* trigger;

public:
	MeteorGroupAI();
	MeteorGroupAI(Vec3 position, int radius);
	~MeteorGroupAI();

	void think(EntityManager *em, int myList, int myIndex);
};

class MeteorGroup: public Entity {
public:
	MeteorGroup();
	MeteorGroup(int id, int numMeteors);
	void setTriggerVolume(Vec3 position, int radius);
	AI* getAI();
	void addMeteor(int index, Vec3 position);

	int id;
	int numMeteors;
	Meteor* *meteors;
	MeteorGroupAI* mind;
};
#endif