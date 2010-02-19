#pragma once
#ifndef METEOR_H
#define METEOR_H

#include "Entity.h"
#include "TriggerVolumeAI.h"

class Meteor: public Entity {
	AI* mind;

public:
	Meteor ();
	~Meteor ();

	void setInput (Input::Arrow dir, bool isKeyDown);
	bool* getInput ();
	AI* getAI ();
};

#endif
