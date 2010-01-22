#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <iostream>
#include "Entity.h"

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

	virtual void update(void) = 0;

};

#endif //VEHICLE_H