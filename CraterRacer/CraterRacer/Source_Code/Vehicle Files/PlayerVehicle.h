#pragma once
#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Vehicle.h"

//---------------------------------------------------------
//		CLASS: PlayerVehicle
//
//		This represents a vehicle object that is controlled
//		by a player.
//---------------------------------------------------------

class PlayerVehicle: public Vehicle 
{

public:

	//Public interface ------------------------------------

	PlayerVehicle( void ) { mind = new VehicleAI(); }
};

#endif //PLAYERVEHICLE_H
