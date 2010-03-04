#pragma once
#ifndef METEOR_H
#define METEOR_H

#include "Entity.h"
#include "TriggerVolumeAI.h"


//---------------------------------------------
// Class: MeteorAI
//---------------------------------------------

class MeteorAI : public AI {
public:
	MeteorAI ();
	~MeteorAI ();

	void think (EntityManager *em, int myList, int myIndex);
	void informOfCollision ();
	void informOfTrigger ();
};


//---------------------------------------------
// Class: Meteor
//---------------------------------------------

class Meteor : public Entity {
private:
	MeteorAI* mind;

public:
	Meteor ();
	Meteor (Vec3 target);
	~Meteor ();

	AI* getAI ();
	void informOfCollision ();
	void informOfTrigger ();

	Vec3 target;
};


#endif
