/*
 * The Player Vehicle class.  This class extends Vehicle, which extends Entity and represents
 * a vehicle that is controlled by a player.
*/

#include "PlayerVehicle.h"

//--------------------------------------------------------------------------------------
// Function:  setInput
// Delegates setting the input to mind.
//--------------------------------------------------------------------------------------
void PlayerVehicle::setInput(Input::Arrow dir, bool isKeyDown)
{
	mind.setInput(dir, isKeyDown);
}

//--------------------------------------------------------------------------------------
// Function: getInput
// Delegates getting the input to mind.
//--------------------------------------------------------------------------------------
bool* PlayerVehicle::getInput()
{
	return mind.getInput();
}

//--------------------------------------------------------------------------------------
// Function: getButtons
// Delegates getButtons to mind.
//--------------------------------------------------------------------------------------
bool* PlayerVehicle::getButtons()
{
	return mind.getButtons();
}

//--------------------------------------------------------------------------------------
// Function: resetInput
// Delegates resetting the input to mind.
//--------------------------------------------------------------------------------------
void PlayerVehicle::resetInput()
{
	mind.resetInput();
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Delegates the information to the mind. (XBox version)
//--------------------------------------------------------------------------------------
void PlayerVehicle::setDir(float x, Input::Button button)
{
	mind.setDir(x, button);
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Delegates the information to the mind. (XBox version)
//--------------------------------------------------------------------------------------
void PlayerVehicle::setDir(float x)
{
	mind.setDir(x);
}

//--------------------------------------------------------------------------------------
// Function: setDir
// Delegates the information to the mind. (Keyboard version)
//--------------------------------------------------------------------------------------
void PlayerVehicle::setDir(Input::Arrow dir, bool isKeyDown)
{
	mind.setDir(dir, isKeyDown);
}