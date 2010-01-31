/*
The input system.
*/

#include "Input.h"

/*
The constructor for Input. Just sets all the values to false.
*/
Input::Input() 
{
	reset();
}

/*
Makes all the directions false.
*/
void Input::reset()
{
	inputs[Arrow::LEFT] = false;
	inputs[Arrow::UP] = false;
	inputs[Arrow::RIGHT] = false;
	inputs[Arrow::DOWN] = false;
}

/*
Set the direction to true if the key is currently down, and false 
when the player releases the key.
*/
void Input::setInput(Input::Arrow dir, bool isKeyDown)
{
	inputs[dir] = isKeyDown;
}

/*
Returns the directions array.
*/
bool* Input::getInput()
{
	return inputs;
}