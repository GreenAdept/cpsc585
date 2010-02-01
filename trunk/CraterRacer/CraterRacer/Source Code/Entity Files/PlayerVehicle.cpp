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


void PlayerVehicle::setDir(Vec3 vec)
{
	mind.setDir(vec);
}

//--------------------------------------------------------------------------------------
// Function: getInput
// Delegates getting the input to mind.
//--------------------------------------------------------------------------------------
bool* PlayerVehicle::getInput()
{
	return mind.getInput();
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