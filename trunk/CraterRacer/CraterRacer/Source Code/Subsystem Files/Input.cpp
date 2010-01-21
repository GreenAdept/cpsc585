/*
The input system.
*/

#include "Input.h"

Input::Input() 
{
	inputs[Arrow::LEFT] = false;
	inputs[Arrow::UP] = false;
	inputs[Arrow::RIGHT] = false;
	inputs[Arrow::DOWN] = false;
}