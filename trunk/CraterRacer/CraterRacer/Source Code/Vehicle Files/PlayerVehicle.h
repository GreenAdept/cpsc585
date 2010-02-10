#pragma once
#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Vehicle.h"
#include "DebugWriter.h"

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

	PlayerVehicle( void ) { }

	void update( void );

protected:
	DebugWriter writer;
};

#endif //PLAYERVEHICLE_H