#pragma once
#ifndef AIVEHICLE_H
#define AIVEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Vehicle.h"
#include "VehicleAI.h"


//---------------------------------------------------------
//		CLASS: AIVehicle
//
//		This represents a vehicle object that is controlled
//		by our game algorithms.
//---------------------------------------------------------

class AIVehicle: public Vehicle {
	VehicleAI mind;

public:
	//Public interface -----------------------------------------

	AIVehicle () { }
	AI* getAI () { return &mind; }
};


#endif
