#pragma once
#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	enum Arrow { LEFT, UP, RIGHT, DOWN };

	Input(void);

private:
	bool inputs[4];
};

#endif