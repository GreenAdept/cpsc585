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

void PlayerVehicle::setInput(Input::Arrow dir, bool isKeyDown)
{
	mind.setInput(dir, isKeyDown);
}

bool* PlayerVehicle::getInput()
{
	return mind.getInput();
}

void PlayerVehicle::resetInput()
{
	mind.resetInput();
}