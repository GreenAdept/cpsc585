#pragma once
#ifndef GAME_OBJ_H
#define GAME_OBJ_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Constants.h"
#include "Resource.h"
#include <iostream>
#include <string>
#include <vector>
#include "SceneLoader.h"
#include "Clock.h"
#include "VictoryCalculator.h"

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
	bool isPaused		 ( );
	void recordTime		 ( );
	void processInput	 ( float fElapsedTime );
	void addInput		 ( bool isKeyDown, UINT virtualKeyCode );
	void simulate		 ( float fElapsedTime );
	void render			 ( Device* device, Renderer* renderer, int playerID );
	void processCallback ( ProcessType type, Device* d=NULL , const D3DSURFACE_DESC* b=NULL);
	
	//Getter's and setter's
	string		 getTime			( );
	float		 getVehicleSpeed	( int playerNum );
	SceneObjects getSceneObjects	( );
	void		 setSceneObjects	( SceneObjects& objs );
	bool		 isFinished			( );

	VictoryCalculator           m_Victory;
	EntityManager*				m_Entities;		// collection of all entities
	Simulator*					m_Simulator;
	Clock						m_Clock;

private:
	//Date Members ---------------------------------------

	DebugWriter*				m_Debugger;
	VarLoader*					m_VarLoader;
	vector<GameCamera*>			m_Cameras;		// main camera to follow behind vehicle
	vector<XBox360Controller*>	m_Controllers;	//first player controller will have id# 0
	bool						m_bIsFinished;
};

#endif GAME_OBJ_H 