/*The vehicle is described as follows:
		
			front
		-------------
wheel 0	|			| wheel 1
		|			|
		|			|
		|			|
		|			|
wheel 3 |			| wheel 2
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

	void update			( Vec3 newPosition, Vec3 velocity, Matrix matBody );
	void update			( Vec3 pos, Vec3 vel, Matrix body, Matrix w0, Matrix w1, Matrix w2, Matrix w3 );
	void initialize		( Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename );

	// Setter's
	void  setMass		( int mass );
	void  setVelocity	( Vec3 velocity );

	// Getter's
	int	  getMass		( );
	Vec3  getVelocity	( );
	Input *getInputObj  ( );
	Vec3  getDirection	( );

	Wheel m_Wheels[4];

protected:
	Input input;
	Vec3  velocity;	//m/s
	int   mass;		//kg
};

#endif //VEHICLE_H