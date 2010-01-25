/*
* The AI Vehicle class.  This class extends Vehicle, which extends Entity and represents
* a vehicle controlled by our game algorithms.
*/

#include "AIVehicle.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the AI vehicle and sends the device to entity base class to initialize
// its renderable.
//--------------------------------------------------------------------------------------
AIVehicle::AIVehicle( Device* device ): Vehicle( device )
{

}


//--------------------------------------------------------------------------------------
// Function:  update
// Will update the position/orientation of the vehicle eventually.
//--------------------------------------------------------------------------------------
void AIVehicle::update( ) 
{
}


AI* AIVehicle::getAI () {
	mind.setPosition (m_vPosition);
	//insert more data brokering here
	return &mind;
}