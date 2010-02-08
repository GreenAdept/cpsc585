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

#include "EntityManager.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Simulator.h"
#include "Input.h"
#include "GameCamera.h"
#include "DebugWriter.h"
#include "Terrain.h"
#include "XBox360Controller.h"


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

	void think           ( void );
	void processInput	 ( float fElapsedTime );
	void addInput		 ( bool isKeyDown, UINT virtualKeyCode );
	void simulate		 ( float fElapsedTime );
	void render			 ( Device* device );
	void initGame		 ( Device* device, const D3DSURFACE_DESC* );
	void processCallback ( ProcessType type, Device* d=NULL , const D3DSURFACE_DESC* b=NULL);

private:
	//Date Members ---------------------------------------

	EntityManager    m_Entities;        // collection of all entities
	Renderer*		 m_Renderer;		// rendering subsystem
	Input			 m_InputManager;	// input management subsystem
	GameCamera		 m_Camera;		// main camera to follow behind vehicle
	Simulator*		 m_Simulator;
	DebugWriter		 debug;
	//Terrain*		 m_Terrain;
	XBox360Controller* m_Controller1; //first player controller will have id# 0
};

#endif GAME_OBJ_H 