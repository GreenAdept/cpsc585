//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//--------------------------------------------------------------------------------------
#include "GameObj.h"
#include "MessageManager.h"


//--------------------------------------------------------------------------------------
// Function:  CONSTRUCTOR
//--------------------------------------------------------------------------------------
GameObj::GameObj( )
{
	m_Debugger  = new DebugWriter( );
	m_Simulator = new Simulator( );
	m_VarLoader = new VarLoader( );
	m_Entities	= new EntityManager();
	m_bIsFinished = false;
}


//--------------------------------------------------------------------------------------
// Function:  DESTRUCTOR 
//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	if( m_Simulator )
		delete m_Simulator;

	for( int i=0; i < m_Controllers.size(); i++ )
		delete m_Controllers[i];

	if( m_Debugger )
		delete m_Debugger;

	if( m_VarLoader )
		delete m_VarLoader;

	if( m_Entities )
		delete	m_Entities;

	for( int i=0; i < m_Cameras.size(); i++ )
		delete m_Cameras[i];
}


//--------------------------------------------------------------------------------------
// Function: getTime
// Returns the game clock in the format mm:ss:msms
//--------------------------------------------------------------------------------------
string GameObj::getTime()
{
	return m_Clock.getFormattedTime();
}


//--------------------------------------------------------------------------------------
// Function:  getSceneObjects
// This function gets game objects and returns them in a single SceneObjects variable.
//--------------------------------------------------------------------------------------
SceneObjects GameObj::getSceneObjects( )
{
	SceneObjects objs;
	
	objs.entityManager	= m_Entities;
	objs.gameCameras	= m_Cameras;
	objs.simulator		= m_Simulator;
	objs.debugger		= m_Debugger;
	objs.varLoader		= m_VarLoader;
	objs.controllers	= m_Controllers;

	return objs;
}


//--------------------------------------------------------------------------------------
// Function:  setSceneObjects
// This function sets the game cameras and controllers.
//--------------------------------------------------------------------------------------
void GameObj::setSceneObjects( SceneObjects& objs )
{	
	m_Cameras = objs.gameCameras;
	m_Controllers = objs.controllers;
}


//--------------------------------------------------------------------------------------
// Function: isPaused
//--------------------------------------------------------------------------------------
bool GameObj::isPaused( )
{
	return m_Simulator->isPaused();
}


//--------------------------------------------------------------------------------------
// Function: getVehicleSpeed
//--------------------------------------------------------------------------------------
float GameObj::getVehicleSpeed( int playerNum )
{
	vector<Vehicle*> vehicles = m_Entities->getVehicles();

	return vehicles[ playerNum ]->getSpeed( );
}

//--------------------------------------------------------------------------------------
// Function: addInput
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{
	Input* v = m_Entities->getPlayerInputObj( PLAYER1 );

	v->setController( false );

	switch (virtualKeyCode)
	{
	case VK_LEFT:
		//add left-ward force to player vehicle
		v->setDir(Input::Arrow::LEFT, isKeyDown);
		break;
	case VK_UP:
		//add forward force to player vehicle
		v->setDir(Input::Arrow::UP, isKeyDown);
		break;
	case VK_RIGHT:
		//add right-ward force to player vehicle
		v->setDir(Input::Arrow::RIGHT, isKeyDown);
		break;
	case VK_DOWN:
		//add backward force to player vehicle
		v->setDir(Input::Arrow::DOWN, isKeyDown);
		break;
	case 68: //D is pressed -> drop car
		v->setKey(Input::Key::D_KEY, isKeyDown);
		break;
	case 83: //S is pressed -> stop vehicle
		v->setKey(Input::Key::S_KEY, isKeyDown);
		break;
	case 76: //L is pressed -> load file
		if (isKeyDown)
		{
			m_VarLoader->loadVars( m_Simulator );
			break;
		}
	default:
		break;
	}
}


//--------------------------------------------------------------------------------------
// Function: think
//--------------------------------------------------------------------------------------
void GameObj::think ()
{
	vector<AI*> minds;
	vector<MeteorGroup*> meteorGroups;

	minds = m_Entities->getAIs (PLAYERS);
	for (int i=0; i<minds.size(); i++)
		minds[i]->think( m_Entities, PLAYERS, i );

	minds = m_Entities->getAIs (COMPUTERS);
	for (int i=0; i<minds.size(); i++)
		minds[i]->think( m_Entities, COMPUTERS, i );

	minds = m_Entities->getAIs (METEORGROUPS);
	for (int i=0; i<minds.size(); i++)
		minds[i]->think( m_Entities, METEORGROUPS, i );


	m_Victory.calculateRanks (m_Entities->getVehicles(), m_Entities->getNumberOfPlayers());

	//check if a player is done and set the flag to make the victory screen load
	//(we will eventually want to start a finishing animation here)
	m_bIsFinished = m_Victory.isGameFinished( );
}


//--------------------------------------------------------------------------------------
// Function: processInput
//--------------------------------------------------------------------------------------
void GameObj::processInput( float fElapsedTime )
{
	for( int i=0; i < m_Controllers.size(); i++ )
	{
		if( m_Controllers[i]->IsConnected() )
		{
			m_Controllers[i]->Update(fElapsedTime);
			Input* v = m_Entities->getPlayerInputObj( i );
			
			v->setDir(m_Controllers[i]->LeftThumbstick.GetX());

			if (m_Controllers[i]->A.WasPressedOrHeld())
			{
				v->setDir(Input::A_BUTTON);
			}
			if (m_Controllers[i]->B.WasPressedOrHeld())
			{
				v->setDir(Input::B_BUTTON);
			}
			if (m_Controllers[i]->Y.WasPressed())
			{
				v->setDir(Input::Y_BUTTON);
			}
			if (m_Controllers[i]->X.WasPressedOrHeld())
			{
				v->setDir(Input::X_BUTTON);
			}

			//Check for acceleration
			if (m_Controllers[i]->RightTrigger.WasPressedOrHeld())
			{
				v->setDir(Input::RT_BUTTON);
				v->setPressure(m_Controllers[i]->RightTrigger.GetValue());

				//Increase vehicle RPM for sound
				g_audioState.nRPM += 15.0f;
                if( g_audioState.nRPM > 3000.0f )
                    g_audioState.nRPM = 3000.0f;
                //g_audioState.pEngine->SetGlobalVariable( g_audioState.iRPMVariable, g_audioState.nRPM );
			}

			//Check for braking
			if (m_Controllers[i]->LeftTrigger.WasPressedOrHeld())
			{
				v->setDir(Input::LT_BUTTON);
				v->setPressure(m_Controllers[i]->LeftTrigger.GetValue());
				if(g_audioState.nRPM > 0)
					g_audioState.nRPM -= 50.0f;
				else
					g_audioState.nRPM -= 25.0f;
				if( g_audioState.nRPM < -3000.0f )
					g_audioState.nRPM = -3000.0f;
				//g_audioState.pEngine->SetGlobalVariable( g_audioState.iRPMVariable, g_audioState.nRPM );
			}
			if (m_Controllers[i]->Start.WasPressed())
			{
				Emit( EPauseGame );//pauseGame( );
			}

			if (m_Controllers[i]->Back.WasPressed())
			{
				//respawn car
				v->setDir(Input::BACK_BUTTON);
			}

			//rear view mirror: while right shoulder is pressed, looking at rear view, else
			//normal view
			if (m_Controllers[i]->RightShoulder.WasPressedOrHeld())
			{
				m_Cameras[i]->setRearView(true);
			}
			else
			{
				m_Cameras[i]->setRearView(false);
			}

			//using xbox controller
			v->setController(true);
		}
	}
}


//--------------------------------------------------------------------------------------
// Function: render
// This function renders the game's scene to the specified device.
//--------------------------------------------------------------------------------------
void GameObj::render( Device* device, Renderer* renderer, int playerID )
{
	vector<Renderable*> renderables = m_Entities->getRenderables();

	int test;

	// pass the renderables off to the renderer to do all the work
	renderer->RenderFrame( device, renderables, m_Cameras, playerID );
}


//--------------------------------------------------------------------------------------
// Function: simulate
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	m_Simulator->simulate( m_Entities->getVehicles(), m_Entities->getMeteorGroups(), fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Function: processCallback
// This function is used to manage game objects that are attached to a device when the
// device is reset, lost or destroyed.  
//--------------------------------------------------------------------------------------
void GameObj::processCallback( ProcessType type, Device* device, const D3DSURFACE_DESC* pBackSurface )
{
	vector<Renderable*> m_Renderables = m_Entities->getRenderables();

	// when the device is destroyed, we want to release all of the memory attached to it
	if( type == ON_DESTROY )
	{
		// make sure the renderables are aware their device is being destroyed, so they
		// can release the appropriate memory
		for( unsigned int i = 0; i < m_Renderables.size(); i++ )
			 m_Renderables[i]->releaseMemory( );
	}

	// when the device is lost, we want to prepare some of objects for destruction or reset
	else if( type == ON_LOST )
	{
		// make sure the renderables are aware their device is lost
		for( unsigned int i = 0; i < m_Renderables.size(); i++ ) 
			 m_Renderables[i]->lostDevice( );
	}

	// when the device is reset the dimensions might have changed, so we want to make sure
	// anything based on the dimensions are adjusted
	else if( type == ON_RESET )
	{
		// re-adjust the camera's projection parameters
		for( int i=0; i < m_Cameras.size(); i++ )
			m_Cameras[i]->updateWindow (pBackSurface);

		// let the renderables know their device was reset
		for( unsigned int i = 0; i < m_Renderables.size(); i++ ) 
			 m_Renderables[i]->resetDevice( device );
	}
}

void GameObj::vibrate(int controllerID, double intensity, double duration)
{
	if (m_Controllers.size() > controllerID) {
		intensity = intensity / 100;
		if (duration == 0)
		{
			m_Controllers[controllerID]->StopVibration();
		}
		duration = duration / 100;
		m_Controllers[controllerID]->Vibrate(intensity*0.75, intensity, duration);
	}
}
