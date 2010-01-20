#pragma once
#ifndef GAME_OBJ_H
#define GAME_OBJ_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "..\Utility Files\Constants.h"
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


//--------------------------------------------------------
//		CLASS: GameObj
//--------------------------------------------------------

class GameObj
{
public:
	//Public Interface -----------------------------------

	GameObj( );
	~GameObj( );

	void processInput	(  );
	void addInput		( bool isKeyDown, UINT virtualKeyCode );
	void simulate		( float fElapsedTime );
	void render			( Device* device );
	void initGame		( Device* device, const D3DSURFACE_DESC* );
	void processCallback( ProcessType type, Device* d=NULL , const D3DSURFACE_DESC* b=NULL);

private:
	//Date Members ---------------------------------------

	vector<Entity*> m_Entities;
	Renderer*		m_Renderer;
	Input			m_InputManager;
	MCamera			m_Camera;
	//Simulator		m_Simulator;
};

#endif GAME_OBJ_H