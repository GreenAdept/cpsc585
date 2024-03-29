//--------------------------------------------------------------------------------------
// The input system.
//--------------------------------------------------------------------------------------

#include "Input.h"

//--------------------------------------------------------------------------------------
// Constructor: Input
// The constructor for Input. Just sets all the values to false.
//--------------------------------------------------------------------------------------
Input::Input() 
{
	reset();
}

//--------------------------------------------------------------------------------------
// Function: reset
// Resets the inputs, buttons, and xBoxController to false
//--------------------------------------------------------------------------------------
void Input::reset()
{
	for (int i = 0; i < 7; i++)
	{
		inputs[i] = false;
		keys[i] = false;
		buttons[i] = false;
	}

	xBoxController = false;
}

//--------------------------------------------------------------------------------------
// Function: setInput
// Set the direction to true if the key is currently down, and false 
// when the player releases the key.
//--------------------------------------------------------------------------------------
void Input::setInput(Input::Arrow dir, bool isKeyDown)
{
	inputs[dir] = isKeyDown;
}

//--------------------------------------------------------------------------------------
// Function: getInput
// Returns the inputs array.
//--------------------------------------------------------------------------------------
bool* Input::getInput()
{
	return inputs;
}

//--------------------------------------------------------------------------------------
// Function: getButtons
// Returns the buttons array. If using the keyboard, buttons[A_BUTTON] and
// buttons[B_BUTTON] have to be set manually from inputs due to an unknown bug.
//--------------------------------------------------------------------------------------
bool* Input::getButtons()
{
	if (!xBoxController)
	{
		buttons[Button::RT_BUTTON] = inputs[Arrow::UP];
		buttons[Button::LT_BUTTON] = inputs[Arrow::DOWN];
		buttons[Button::BACK_BUTTON] = keys[Key::D_KEY];
		setPressure(1.0f);
	}

	return buttons;
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
// Gets the x value of the left thumbstick. If using the keyboard, then return -1 if 
// the left key is pressed, 1 if the right key is pressed, or 0 if neither are pressed.
// This is done to avoid an unknown bug.
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

//--------------------------------------------------------------------------------------
// Function: setDir
// Called from GameObj.processInput when no buttons are pushed, so we only look at
// the left stick. Sets all the buttons to false.
//--------------------------------------------------------------------------------------
void Input::setDir(float x)
{
	setThumbstick(x);
	for (int i = 0; i < 6; i++)
		buttons[i] = false;
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Called when an xBox button is pushed, sets the corresponding button to true, and
// updates the thumbstick.
//--------------------------------------------------------------------------------------
void Input::setDir(Input::Button button)
{
	buttons[button] = true;
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Called when pressing an Arrow key on the keyboard. Sets the corresponding input
// button to isKeyDown, then updates as necessary depending on the Key pressed.
//--------------------------------------------------------------------------------------
void Input::setDir(Input::Arrow dir, bool isKeyDown)
{
	setInput(dir, isKeyDown);

	switch(dir) {
		case (Arrow::LEFT):
			{
				break;
			}
		case (Arrow::UP):
			{
				buttons[Button::RT_BUTTON] = isKeyDown;
				break;
			}
		case (Arrow::RIGHT):
			{
				break;
			}
		case (Arrow::DOWN):
			{
				buttons[Button::LT_BUTTON] = isKeyDown;
				break;
			}
		default:
			break;
	}

	setPressure(1.0f);
}

void Input::setKey(Input::Key key, bool isKeyDown)
{
	keys[key] = isKeyDown;
}

void Input::setPressure(float p)
{
	pressure = p;
}

float Input::getPressure()
{
	return pressure;
}