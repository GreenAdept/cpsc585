#pragma once
#ifndef GAME_OBJ_H
#define GAME_OBJ_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "..\Utility Files\Constants.h"
#include "Resource.h"
#include <iostream>
#include <vector>
#include "Vehicle.h"
#include "PlayerVehicle.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Simulator.h"
#include "Prop.h"
#include "Input.h"
#include "GameCamera.h"


//-----------------------------------------------------------------
//		CLASS: GameObj
//
//		This class represents our racing game and its components
//		(which include all Rendering, Input Management, Physics, etc.
//		subsystems)
//------------------------------------------------------------------

class GameObj
{
public:
	//Public Interface -----------------------------------

	GameObj( ) {}
	~GameObj( );

	void processInput	 (  );
	void addInput		 ( bool isKeyDown, UINT virtualKeyCode );
	void simulate		 ( float fElapsedTime );
	void render			 ( Device* device );
	void initGame		 ( Device* device, const D3DSURFACE_DESC* );
	void processCallback ( ProcessType type, Device* d=NULL , const D3DSURFACE_DESC* b=NULL);

private:
	//Date Members ---------------------------------------

	vector<Entity*> m_Entities;		// list of all scene objects
	Renderer*		m_Renderer;		// rendering subsystem
	Input			m_InputManager;	// input management subsystem
	GameCamera		m_Camera;		// main camera to follow behind vehicle
	Simulator*		m_Simulator;
};

#endif GAME_OBJ_H 