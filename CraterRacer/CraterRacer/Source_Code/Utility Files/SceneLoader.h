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

	GameObj* startGame	( string filename );	
	void	 initScene	( Device* device, const D3DSURFACE_DESC* backSurface, string filename );

	void processTerrainInfo		( ifstream& file );
	void processVehicleInfo		( ifstream& file );
	void initializeSimulator	( );

private:

	wstring toLPCWSTR				( std::string& s );
	void	processComputerVehicles ( ifstream& file, LPCWSTR meshL, int num );
	void	processPlayerVehicles	( ifstream& file, LPCWSTR meshL, int num );

	//Date Members ---------------------------------------

	Device*				m_Device;
	D3DSURFACE_DESC*	m_BackSurface;
	GameObj*			m_Game;
	SceneObjects		m_Objs;
};

#endif //SCENE_LOADER_H