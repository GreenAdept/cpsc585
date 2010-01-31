/*
* The AI Vehicle class.  This class extends Vehicle, which extends Entity and represents
* a vehicle controlled by our game algorithms.
*/

#include "AIVehicle.h"


AI* AIVehicle::getAI ( ) 
{
	mind.setPosition( m_vPosition );
	//insert more data brokering here
	return &mind;
}