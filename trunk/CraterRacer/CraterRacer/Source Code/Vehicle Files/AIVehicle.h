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
	VehicleAI mind;
public:

	//Public interface -----------------------------------------

	AIVehicle ( void ){ }
	AI* getAI (void);

};
#endif