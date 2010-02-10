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
	static enum Button { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON, LT_BUTTON, RT_BUTTON };

	Input();
	void setInput(Arrow dir, bool isKeyDown);
	bool* getInput();
	bool* getButtons();
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();
	void reset();

	void setDir(float x, Input::Button button);
	void setDir(float x);
	void setDir(Input::Arrow, bool isKeyDown);

private:
	bool inputs[6];
	bool buttons[6];

	bool xBoxController;
	float x;

	DebugWriter m_Debugger;
};

#endif