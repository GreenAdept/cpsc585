/*
 The Input system.
*/
#pragma once
#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	//enum class of directions
	static enum Arrow { LEFT, UP, RIGHT, DOWN };

	Input(void);
	void setInput(Arrow dir, bool isKeyDown);
	bool* getInput();
	void reset();

private:
	bool inputs[4];
};

#endif