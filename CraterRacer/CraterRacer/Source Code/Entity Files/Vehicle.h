#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <iostream>
#include "Entity.h"
#include "Input.h"
#include "VehicleAI.h"

using namespace std;


//---------------------------------------------------------
//		CLASS: Vehicle
//
//		This is the base class of a vehicle object.  It
//		is abstract so cannot be instantiated by itself.
//---------------------------------------------------------

class Vehicle: public Entity 
{

public:

	// Public interface ------------------------------------

	Vehicle( void ) {}
	Vehicle( Device* );

	//virtual void update(void) = 0;
	//virtual void setInput(Input::Arrow dir, bool isKeyDown) = 0;
	//virtual bool* getInput() = 0;

protected:
	VehicleAI mind;

};

#endif //VEHICLE_H