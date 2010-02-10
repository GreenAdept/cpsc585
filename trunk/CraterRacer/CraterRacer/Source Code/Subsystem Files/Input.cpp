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
	m_Debugger.writeToFile("buttons in input.getButtons");
	
	buttons[Button::A_BUTTON] = inputs[Arrow::UP];
	buttons[Button::B_BUTTON] = inputs[Arrow::DOWN];

	for (int i = 0; i < 4; i++)
		if (buttons[i])
			m_Debugger.writeToFile("true");
		else
			m_Debugger.writeToFile("false");

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
	
	switch(dir) {
		case (Arrow::LEFT):
			{
				setThumbstick(-1.0);
				break;
			}
		case (Arrow::UP):
			{
				buttons[Button::A_BUTTON] = isKeyDown;
				inputs[Arrow::UP] = isKeyDown;
				//debug: this works
				if (buttons[Button::A_BUTTON])
					m_Debugger.writeToFile("A_True");
				break;
			}
		case (Arrow::RIGHT):
			{
				setThumbstick(1.0);
				break;
			}
		case (Arrow::DOWN):
			{
				m_Debugger.writeToFile("Pressing B");
				buttons[Button::B_BUTTON] = isKeyDown;
				inputs[Arrow::DOWN] = isKeyDown;
				break;
			}
		default:
			break;
	}
/*	//debug: this looks ok
	m_Debugger.writeToFile("buttons in input.setDir");
	for (int i = 0; i < 4; i++)
		if (buttons[i])
			m_Debugger.writeToFile("true");
		else
			m_Debugger.writeToFile("false");
*/
}