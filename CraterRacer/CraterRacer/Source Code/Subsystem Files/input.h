/*
 The Input system.
*/
#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Constants.h"
#include "DebugWriter.h"

class Input
{
public:
	//enum class of directions
	static enum Arrow { LEFT, UP, RIGHT, DOWN };
	static enum Button { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON };

	Input();
	void setInput(Arrow dir, bool isKeyDown);
	bool* getInput();
	bool* getButtons();
	Vec3 getDir();
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();
	void reset();

	void setDir(float x, Input::Button button);
	void setDir(float x);
	void setDir(Input::Arrow, bool isKeyDown);

private:
	bool inputs[4];
	bool buttons[4];

	Vec3 direction;
	bool reversing; //flag to show it is reversing
	bool xBoxController;
	float x;
	DebugWriter m_Debugger;
};

#endif