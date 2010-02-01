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
	for (int i = 0; i < 4; i++)
	{
		inputs[i] = false;
	}

	for (int i = 0; i < 8; i++)
	{
		buttons[i] = false;
	}

	direction = Vec3(0.0, 0.0, 0.0);
}

/*
Set the direction to true if the key is currently down, and false 
when the player releases the key.
*/
void Input::setInput(Input::Arrow dir, bool isKeyDown)
{
	inputs[dir] = isKeyDown;
}

void Input::setDir(Vec3 vec)
{
	direction = vec;
}

/*
Returns the directions array.
*/
bool* Input::getInput()
{
	return inputs;
}

Vec3 Input::getDir()
{
	return direction;
}