/*The vehicle is described as follows:
		
			front
		-------------
wheel 0	|			| wheel 1
		|			|
		|			|
		|			|
		|			|
wheel 2 |			| wheel 3
		-------------
			back


Wheel angle (degrees):
- Angle is defined between -45 and 45 only.

				0
	          .    .
	      .     :      .
  -45  .        :         .  45
	 .			:		    .
    .           :            . 

*/

#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#define NOMINMAX
#include "Entity.h"
#include "Input.h"
#include "VehicleAI.h"
#include "Wheel.h"

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
	void setMass(int mass);
	int getMass();
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();

	virtual void setInput(Input::Arrow dir, bool isKeyDown) = 0;
	virtual void setDir(Vec3 vec) = 0;
	virtual bool* getInput() = 0;
	virtual bool* getButtons() = 0;
	virtual Vec3 getDir() = 0;
	virtual void resetInput() = 0;


	virtual void drive(Vec3 dir, bool accelerate, bool decelerate) = 0;

	Wheel m_Wheels[4];

protected:
	VehicleAI mind;
	Vec3 velocity; //m/s
	int mass; //kg
};

#endif //VEHICLE_H