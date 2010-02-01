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
	static enum Buttons { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON, LEFT_SHOULDER, RIGHT_SHOULDER, LEFT_TRIGGER, RIGHT_TRIGGER };

	Input(void);
	void setInput(Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	Vec3 getDir();
	void reset();

private:
	bool inputs[4];
	Vec3 direction;
	bool buttons[8];
};

#endif