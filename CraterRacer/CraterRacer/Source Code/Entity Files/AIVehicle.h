#pragma once
#ifndef AIVEHICLE_H
#define AIVEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Vehicle.h"


//---------------------------------------------------------
//		CLASS: AIVehicle
//
//		This represents a vehicle object that is controlled
//		by our game algorithms.
//---------------------------------------------------------

class AIVehicle: public Vehicle 
{
public:

	//Public interface -----------------------------------------

	AIVehicle( void ){ }
	AIVehicle( Device* );

	void update(void);

};
#endif