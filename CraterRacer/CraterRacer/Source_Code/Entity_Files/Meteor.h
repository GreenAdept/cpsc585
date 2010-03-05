#pragma once
#ifndef METEOR_H
#define METEOR_H

#include "Entity.h"
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

public:
	Meteor ();
	~Meteor ();

	AI* getAI ();
	void informOfCollision ();
	void informOfTrigger ();
};


#endif
