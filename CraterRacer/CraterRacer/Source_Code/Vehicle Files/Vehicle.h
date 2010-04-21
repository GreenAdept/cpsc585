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
#include "VehicleAI.h"
#include "Input.h"
#include "Wheel.h"
#include "Clock.h"

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

	Vehicle                 ( ) { mind = 0; }
	virtual ~Vehicle        ( ) { delete mind; }

	void	update			( Vec3 newPosition, Vec3 velocity, Matrix matBody );
	void	update			( Vec3 pos, Vec3 vel, Matrix body, Matrix w0, Matrix w1, Matrix w2, Matrix w3 );
	void	initialize		( Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename );
	Vec3	lastPassedWP    ( );
	Vec3	nextWP			( );

	// Setter's
	void	setMass			( int mass );
	void	setVelocity		( Vec3 velocity );
	void	setTimeElapsed	( float time );

	// Getter's
	int		getMass			( );
	Vec3	getVelocity		( );
	Input	*getInputObj	( );
	Vec3	getDirection	( );
	AI*		getAI			( );
	float	getSpeed		( );

	bool	isReversing		( );
	void	setReverse		( bool r );
	void	setPlayerNum	( int num );

	int		getRank			( );
	void	setRank			( int rank );
	bool	isPlayer		( );

	Wheel m_Wheels[4];
	Clock m_clock;

protected:
	Input		input;
	Vec3		velocity;	//m/s
	int			mass;		//kg
	VehicleAI*	mind;
	bool		reversing;
	Vec3		m_vPreviousPosition;
	float		m_fTimeElapsed;
	float		m_fRunningTime;
	float		m_fRunningDistance;
	float		m_fSpeed;
	int			m_iPlayerNumber;
	int			m_iRank;
	bool		m_bIsPlayer;
};



//---------------------------------------------------------
//		CLASS: PlayerVehicle
//
//		This represents a vehicle object that is controlled
//		by a player.
//---------------------------------------------------------

class PlayerVehicle: public Vehicle {

public:
	PlayerVehicle( void ) 
	{
		mind = new VehicleAI();
		m_iRank = -1;
		m_bIsPlayer = true;
	}
};



//---------------------------------------------------------
//		CLASS: AIVehicle
//
//		This represents a vehicle object that is controlled
//		by our game algorithms.
//---------------------------------------------------------

class AIVehicle: public Vehicle {

public:
	AIVehicle( void )
	{
		mind = new CompVehicleAI(); 
		m_iRank = -1;
		m_bIsPlayer = false;
	}
};

#endif //VEHICLE_H