#pragma once
#ifndef CRATER_H
#define CRATER_H

#include "Entity.h"

class Crater: public Entity {
	public:
		Crater(void);
		void setInput(Input::Arrow dir, bool isKeyDown);
		bool* getInput();
};
#endif