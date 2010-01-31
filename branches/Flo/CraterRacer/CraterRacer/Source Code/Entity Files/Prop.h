/*
 The prop class.
*/

#pragma once
#ifndef PROP_H
#define PROP_H

#include "Entity.h"

class Prop: public Entity {
	public:
		Prop(void);
		void setInput(Input::Arrow dir, bool isKeyDown);
		bool* getInput();
};
#endif