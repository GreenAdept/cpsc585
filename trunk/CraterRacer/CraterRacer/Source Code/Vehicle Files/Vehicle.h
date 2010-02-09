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

	void update			( Vec3 newPosition, Vec3 velocity, Matrix matBody, Vec3 lTop, Vec3 rTop, Vec3 lBottom, Vec3 rBottom );
	void update			( Vec3 pos, Vec3 vel, Matrix body, Matrix w0, Matrix w1, Matrix w2, Matrix w3 );
	void initialize		( Device* device, Vec3 pos, LPCWSTR filename );

	// Setter's
	void  setThumbstick	( float x );
	void  setVelocity	( Vec3 velocity );
	void  setController	( bool controller );
	void  setMass		( int mass );

	// Getter's
	int	  getMass		( );
	bool  getController	( );
	Vec3  getVelocity	( );
	float getThumbstick	( );

	// Input functions
	virtual void  setInput	 (Input::Arrow dir, bool isKeyDown) = 0;
	virtual bool* getInput	 ( ) = 0;
	virtual bool* getButtons ( ) = 0;
	virtual Vec3  getDir	 ( ) = 0;
	virtual void  resetInput ( ) = 0;

	virtual void  setDir	 (float x, Input::Button button) = 0;
	virtual void  setDir	 (float x) = 0;
	virtual void  setDir	 ( Input::Arrow dir, bool isKeyDown ) = 0;

	Wheel m_Wheels[4];

protected:
	VehicleAI mind;
	Vec3 velocity;	//m/s
	int mass;		//kg
};

#endif //VEHICLE_H