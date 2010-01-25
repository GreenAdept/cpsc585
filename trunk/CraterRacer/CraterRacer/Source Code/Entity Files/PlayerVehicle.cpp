/*
 * The Player Vehicle class.  This class extends Vehicle, which extends Entity and represents
 * a vehicle that is controlled by a player.
*/

#include "PlayerVehicle.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the player vehicle and sends the device to entity base class to initialize
// its renderable.
//--------------------------------------------------------------------------------------
PlayerVehicle::PlayerVehicle( Device* device ): Vehicle( device )
{

}


//--------------------------------------------------------------------------------------
// Function:  update
// Will update the position/orientation of the vehicle eventually.
//--------------------------------------------------------------------------------------
//void PlayerVehicle::update( ) 
//{
//	
//}

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
// Function: resetInput
// Delegates resetting the input to mind.
//--------------------------------------------------------------------------------------
void PlayerVehicle::resetInput()
{
	mind.resetInput();
}