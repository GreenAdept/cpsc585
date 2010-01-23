#pragma once
#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	static enum Arrow { LEFT, UP, RIGHT, DOWN };

	Input(void);
	void setInput(Arrow dir, bool isKeyDown);
	void reset();

private:
	bool inputs[4];
};

#endif