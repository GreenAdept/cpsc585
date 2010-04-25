#pragma once
#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include "Constants.h"
#include "EntityManager.h"
#include "Simulator.h"
#include "GameCamera.h"
#include "Sound.h"
#include "XBox360Controller.h"
#include "Renderer.h"
#include "Input.h"
#include "DebugWriter.h"
#include "VarLoader.h"
#include "Constants.h"
#include "GameObj.h"
#include "MeteorGroup.h"

using namespace std;


//-----------------------------------------------------------------
//		STRUCT: SceneObject
//-----------------------------------------------------------------

struct SceneObjects
{
	EntityManager*				entityManager;
	Simulator*					simulator;
	vector< GameCamera* >		gameCameras;
	Renderer*					renderer;
	DebugWriter*				debugger;
	VarLoader*					varLoader;
	vector<XBox360Controller*>	controllers;

	SceneObjects ();
};


class GameObj; //forward declaration

//-----------------------------------------------------------------
//		CLASS: SceneLoader
//-----------------------------------------------------------------

class SceneLoader 
{
public:
	//Public Interface -----------------------------------

	SceneLoader ( );
	~SceneLoader( );

	void startGame	( string filename );	
	void initScene	( GameObj** obj );
	void initVars	( Device* device, const D3DSURFACE_DESC* backSurface, string filename );

	void processTerrainInfo		( ifstream& file );
	void processPathInfo		( ifstream& file );
	void processMeteorInfo		( ifstream& file );
	void processCraterInfo		( ifstream& file );
	void processPropInfo		( ifstream& file );
	void processVehicleInfo		( ifstream& file );
	void initializeSimulator	( );

private:

	wstring toLPCWSTR				( std::string& s );
	void	processComputerVehicles ( ifstream& file, int num );
	void	processPlayerVehicles	( ifstream& file, int num );

	//Date Members ---------------------------------------

	Device*				m_Device;
	D3DSURFACE_DESC*	m_BackSurface;
	GameObj*			m_Game;
	SceneObjects		m_Objs;
	string				m_InitFilename;
	vector<Mesh*>		m_Ramps;
};

#endif //SCENE_LOADER_H