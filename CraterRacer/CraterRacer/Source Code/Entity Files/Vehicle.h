#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

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

	Vehicle( void ) { }

	void update(Vec3 newPosition, Vec3 velocity, Matrix mat);

	void setVelocity(Vec3 velocity);
	Vec3 getVelocity();

	//virtual void setInput(Input::Arrow dir, bool isKeyDown) = 0;
	//virtual bool* getInput() = 0;
	virtual Vec3 getDir() = 0;

	virtual void drive(Vec3 dir, bool accelerate, bool decelerate) = 0;
	virtual void drive(Input::Arrow dir, bool isKeyDown) = 0;

protected:
	VehicleAI mind;
	Vec3 velocity;

	//double acceleration; //how fast can this vehicle accelerate?
	//double deceleration; //braking constant?? not sure if we need this
	//double max_speed;
};

#endif //VEHICLE_H