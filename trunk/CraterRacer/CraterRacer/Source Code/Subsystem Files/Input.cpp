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
	reversing = false;
}

/*
Makes all the directions false.
*/
void Input::reset()
{
	for (int i = 0; i < 4; i++)
	{
		inputs[i] = false;
		buttons[i] = false;
	}

	direction = Vec3(0.0, 0.0, 0.0);

	xBoxController = false;
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

bool* Input::getButtons()
{
	//debug: not ok here
	//m_Debugger.writeToFile("buttons in input.getButtons");

	if (!xBoxController)
	{
		buttons[Button::A_BUTTON] = inputs[Arrow::UP];
		buttons[Button::B_BUTTON] = inputs[Arrow::DOWN];
	}
/*
	for (int i = 0; i < 4; i++)
		if (buttons[i])
			m_Debugger.writeToFile("true");
		else
			m_Debugger.writeToFile("false");
*/	
	return buttons;
}

Vec3 Input::getDir()
{
	return direction;
}

//--------------------------------------------------------------------------------------
// Function: setController
// Sets whether the input is keyboard or Xbox.
//--------------------------------------------------------------------------------------
void Input::setController(bool controller)
{
	xBoxController = controller;
}

//--------------------------------------------------------------------------------------
// Function: getController
// Gets whether the input is keyboard or Xbox.
//--------------------------------------------------------------------------------------
bool Input::getController()
{
	return xBoxController;
}

//--------------------------------------------------------------------------------------
// Function: setThumbstick
// Sets the x value of the left thumbstick.
//--------------------------------------------------------------------------------------
void Input::setThumbstick(float x)
{
	this->x = x;
}

//--------------------------------------------------------------------------------------
// Function: setThumbstick
// Gets the x value of the left thumbstick.
//--------------------------------------------------------------------------------------
float Input::getThumbstick() {
	if (!xBoxController) {
		if (inputs[Arrow::LEFT])
			return -1.0;
		else if (inputs[Arrow::RIGHT])
			return 1.0;
		else
			return 0;
	}
	//else return the controller value
	return x;
}

//should be called only (from gameobj) when no buttons are pressed
void Input::setDir(float x)
{
	setThumbstick(x);
	for (int i = 0; i < 4; i++)
		buttons[i] = false;
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Determines which direction to go in, and force if applicable.
//--------------------------------------------------------------------------------------
void Input::setDir(float x, Input::Button button)
{
	buttons[button] = true;
	setThumbstick(x);
}

void Input::setDir(Input::Arrow dir, bool isKeyDown)
{
	setInput(dir, isKeyDown);
	switch(dir) {
		case (Arrow::LEFT):
			{
				setThumbstick(-1.0);
				break;
			}
		case (Arrow::UP):
			{
				buttons[Button::A_BUTTON] = isKeyDown;
				break;
			}
		case (Arrow::RIGHT):
			{
				setThumbstick(1.0);
				break;
			}
		case (Arrow::DOWN):
			{
				buttons[Button::B_BUTTON] = isKeyDown;
				break;
			}
		default:
			break;
	}
}