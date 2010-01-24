/*
The input system.
*/

#include "Input.h"

Input::Input() 
{
	reset();
}

void Input::reset()
{
	inputs[Arrow::LEFT] = false;
	inputs[Arrow::UP] = false;
	inputs[Arrow::RIGHT] = false;
	inputs[Arrow::DOWN] = false;
}

void Input::setInput(Input::Arrow dir, bool isKeyDown)
{
	inputs[dir] = isKeyDown;
}

bool* Input::getInput()
{
	return inputs;
}