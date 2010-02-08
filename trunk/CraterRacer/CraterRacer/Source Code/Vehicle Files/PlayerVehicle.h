#pragma once
#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Vehicle.h"

//---------------------------------------------------------
//		CLASS: PlayerVehicle
//
//		This represents a vehicle object that is controlled
//		by a player.
//---------------------------------------------------------

class PlayerVehicle: public Vehicle 
{

public:

	//Public interface ------------------------------------

	PlayerVehicle( void ) { }

	void update( void );
	void setInput(Input::Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	bool* getButtons();
	Vec3 getDir();
	void resetInput();

	void drive(Vec3 dir, bool accelerate, bool decelerate);
	void drive(Input::Arrow dir, bool isKeyDown);
};

#endif //PLAYERVEHICLE_H