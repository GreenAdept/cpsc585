#pragma once
#ifndef METEOR_H
#define METEOR_H

#include "Entity.h"

class Meteor: public Entity {
	public:
		Meteor(void);
		void setInput(Input::Arrow dir, bool isKeyDown);
		bool* getInput();
};
#endif