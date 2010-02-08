/*
 The Input system.
*/
#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Constants.h"
//#include "DebugWriter.h"

class Input
{
public:
	//enum class of directions
	static enum Arrow { LEFT, UP, RIGHT, DOWN };
	static enum Button { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON };

	Input();
	void setInput(Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	bool* getButtons();
	Vec3 getDir();
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();
	void reset();

	void drive(Vec3 dir, bool accelerate, bool decelerate, Vec3 velocity);
	void drive(Input::Arrow dir, bool isKeyDown);

private:
	bool inputs[4];
	bool buttons[4];

	Vec3 direction;
	bool reversing; //flag to show it is reversing
	bool xBoxController;
	float x;
};

#endif