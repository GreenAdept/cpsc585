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

bool* PlayerVehicle::getButtons()
{
	/*
	bool* buttons = mind.getButtons();
	writer.writeToFile("buttons in player vehicle");
	for (int i = 0; i < 4; i++)
		if (buttons[i])
			writer.writeToFile("true");
		else
			writer.writeToFile("false");
			*/
	return mind.getButtons();
}

Vec3 PlayerVehicle::getDir()
{
	return mind.getDir();
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
// Function: drive
// Delegates the information to the mind.
//--------------------------------------------------------------------------------------
void PlayerVehicle::setDir(float x, Input::Button button)
{
	mind.setDir(x, button);
}

void PlayerVehicle::setDir(float x)
{
	mind.setDir(x);
}

void PlayerVehicle::setDir(Input::Arrow dir, bool isKeyDown)
{
	mind.setDir(dir, isKeyDown);
}