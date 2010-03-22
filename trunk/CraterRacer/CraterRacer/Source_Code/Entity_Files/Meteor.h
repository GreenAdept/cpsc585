#pragma once
#ifndef METEOR_H
#define METEOR_H

#include "Entity.h"
#include "Crater.h"
#include "TriggerVolumeAI.h"


//---------------------------------------------
// Class: MeteorAI
//---------------------------------------------

class MeteorAI : public AI {
	TriggerCylinder* trigger;

public:
	MeteorAI (float radius);
	~MeteorAI ();

	void think (EntityManager *em, int myList, int myIndex);
	void informOfCollision ();
	void informOfTrigger ();
};


//---------------------------------------------
// Class: Meteor
//---------------------------------------------

class Meteor : public Entity {
	MeteorAI* mind;
	Vec3 target;
	Crater* toSpawn;

public:
	Meteor ();
	~Meteor ();

	AI*     getAI ();

	Vec3    getTarget ();
	void    setTarget (Vec3 t);
	Crater* getCraterToSpawn ();
	void    setCraterToSpawn (Crater* c);

	void    informOfCollision ();
	void    informOfTrigger ();
};


#endif
