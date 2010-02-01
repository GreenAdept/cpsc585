/*
 The Input system.
*/
#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Constants.h"

class Input
{
public:
	//enum class of directions
	static enum Arrow { LEFT, UP, RIGHT, DOWN };

	Input(void);
	void setInput(Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	Vec3 getDir();
	void reset();

private:
	bool inputs[4];
	Vec3 direction;
};

#endif