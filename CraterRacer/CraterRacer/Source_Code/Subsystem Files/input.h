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
	static enum Key { D_KEY, R_KEY, S_KEY };
	static enum Button { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON, LT_BUTTON, RT_BUTTON, BACK_BUTTON };

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
	void setPressure(float p);
	float getPressure();

	void setKey(Input::Key key, bool isKeyDown);

private:
	bool inputs[7];
	bool keys[7];
	bool buttons[7];

	bool xBoxController;
	float x;
	float pressure;

	DebugWriter m_Debugger;
};

#endif