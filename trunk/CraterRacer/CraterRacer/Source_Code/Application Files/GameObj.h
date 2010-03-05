#pragma once
#ifndef GAME_OBJ_H
#define GAME_OBJ_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Constants.h"
#include "Resource.h"
#include <iostream>
#include <vector>
#include "SceneLoader.h"
#include "Clock.h"

struct SceneObjects; //forward declaration

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

	GameObj( );
	~GameObj( );

	void think           ( void );
	bool pauseGame		 ( );
	bool isPaused		 ( );
	void processInput	 ( float fElapsedTime );
	void addInput		 ( bool isKeyDown, UINT virtualKeyCode );
	void simulate		 ( float fElapsedTime );
	void render			 ( Device* device );
	void initGame		 ( );
	void startClock		 ( );
	void processCallback ( ProcessType type, Device* d=NULL , const D3DSURFACE_DESC* b=NULL);

	SceneObjects getSceneObjects( );
	void setSceneObjects( SceneObjects& objs );

private:
	//Date Members ---------------------------------------

	EntityManager*				m_Entities;		// collection of all entities
	Renderer*					m_Renderer;		// rendering subsystem
	Simulator*					m_Simulator;
	DebugWriter*				m_Debugger;
	VarLoader*					m_VarLoader;
	vector<GameCamera*>			m_Cameras;		// main camera to follow behind vehicle
	vector<XBox360Controller*>	m_Controllers;	//first player controller will have id# 0

	Clock						m_clock;
};

#endif GAME_OBJ_H 