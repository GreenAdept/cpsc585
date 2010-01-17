#pragma once
#ifndef GAME_OBJ_H
#define GAME_OBJ_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "DXUT.h"
#include "resource.h"
#include <iostream>
#include <vector>
#include "vehicle.h"
#include "playerVehicle.h"
#include "renderer.h"
#include "renderable.h"
//#include "simulator.h"
#include "prop.h"
#include "input.h"


typedef D3DXVECTOR3 Vec3;

//--------------------------------------------------------
//		CLASS: GameObj
//--------------------------------------------------------

class GameObj
{
public:
	//Public Interface -----------------------------------

	GameObj();

	void processInput(  );
	void addInput( UINT uMsg, WPARAM wParam );
	void simulate( float fElapsedTime );
	void render(void);
	/*void sound(void);*/

private:
	//Date Members ---------------------------------------

	vector<Entity*> entities;
	Renderer renderer;
	Input input;
	//Simulator simulator;
};

#endif GAME_OBJ_H