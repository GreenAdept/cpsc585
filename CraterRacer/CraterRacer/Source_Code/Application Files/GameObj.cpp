//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//--------------------------------------------------------------------------------------
#include "GameObj.h"


//--------------------------------------------------------------------------------------
// Function:  Constructor
//--------------------------------------------------------------------------------------
GameObj::GameObj( )
{
	m_Debugger  = new DebugWriter( );
	m_Renderer  = new Renderer( );
	m_Simulator = new Simulator( );
	m_VarLoader = new VarLoader( );
	m_Entities	= new EntityManager();
}


//--------------------------------------------------------------------------------------
// Function:  initGame
// Here we initialize the game's camera and scene.  Eventually we want to read all of this
// in via file IO.  
//--------------------------------------------------------------------------------------
void GameObj::initGame( )
{
	
}

//--------------------------------------------------------------------------------------
// Function: startClock
// Start the game clock.
//--------------------------------------------------------------------------------------
void GameObj::startClock()
{
	m_clock.start();
}

//--------------------------------------------------------------------------------------
// Function:  getSceneObjects
//--------------------------------------------------------------------------------------
SceneObjects GameObj::getSceneObjects( )
{
	SceneObjects objs;
	
	objs.entityManager	= m_Entities;
	objs.gameCameras	= m_Cameras;
	objs.simulator		= m_Simulator;
	objs.renderer		= m_Renderer;
	objs.debugger		= m_Debugger;
	objs.varLoader		= m_VarLoader;
	objs.controllers	= m_Controllers;

	return objs;
}

//--------------------------------------------------------------------------------------
// Function:  getSceneObjects
//--------------------------------------------------------------------------------------
void GameObj::setSceneObjects( SceneObjects& objs )
{	
	m_Cameras = objs.gameCameras;
	m_Controllers = objs.controllers;
}

//--------------------------------------------------------------------------------------
// Function:  pause
// pause=true to try to pause simulation
// Returns true if the game pause was toggled
//--------------------------------------------------------------------------------------
bool GameObj::pauseGame()
{
	m_Debugger->writeToFile("pause toggled!");
	bool pause = m_clock.togglePause();
	return m_Simulator->pause(pause);
}


//--------------------------------------------------------------------------------------
// Function: addInput
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{
	Input* v = m_Entities->getPlayerInputObj( 0 );

	v->setController( false );

	switch (virtualKeyCode)
	{
	case VK_LEFT:
		//add left-ward force to player vehicle
		v->setDir(Input::Arrow::LEFT, isKeyDown);
		break;
	case VK_UP:
		//add forward force to player vehicle
		//debug.writeToFile("jldsajflkdwjfoijewojta;lsjg");
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
			if( m_VarLoader->loadVars( m_Simulator ) )
				m_Debugger->writeToFile("Loaded variables successfully");

			else
				m_Debugger->writeToFile("Variables did not load");
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


	m_Victory.calculateRanks (m_Entities->getVehicles());
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
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::A_BUTTON);
			}
			if (m_Controllers[i]->B.WasPressedOrHeld())
			{
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::B_BUTTON);
			}
			if (m_Controllers[i]->Y.WasPressedOrHeld())
			{
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::Y_BUTTON);
			}
			if (m_Controllers[i]->X.WasPressedOrHeld())
			{
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::X_BUTTON);
			}
			if (m_Controllers[i]->RightTrigger.WasPressedOrHeld())
			{
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::RT_BUTTON);
			}
			if (m_Controllers[i]->LeftTrigger.WasPressedOrHeld())
			{
				v->setDir(m_Controllers[i]->LeftThumbstick.GetX(), Input::LT_BUTTON);
			}
			if (m_Controllers[i]->Start.WasPressedOrHeld())
			{
				//m_clock.togglePause();
				//m_Debugger->writeToFile("pause toggled!");
				pauseGame( );
			}

			//using xbox controller
			v->setController(true);
		}
	}

	m_Debugger->writeToFile(m_clock.getFormattedTime());
}

//--------------------------------------------------------------------------------------
// Function: render
// This function renders the game's scene to the specified device.
//--------------------------------------------------------------------------------------
void GameObj::render( Device* device )
{
	vector<Renderable*> renderables = m_Entities->getRenderables();

	int test;

	// pass the renderables off to the renderer to do all the work
	m_Renderer->render( device, renderables, m_Cameras );
}


//--------------------------------------------------------------------------------------
// Function: simulate
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	m_Simulator->simulate( m_Entities->getVehicles(), m_Entities->getMeteorGroups(), fElapsedTime );

}

//--------------------------------------------------------------------------------------
// Function: isPaused
//--------------------------------------------------------------------------------------
bool GameObj::isPaused( )
{
	return m_Simulator->isPaused();
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


//--------------------------------------------------------------------------------------
// Function:  Destructor 
//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	if( m_Simulator )
		delete m_Simulator;

	for( int i=0; i < m_Controllers.size(); i++ )
		delete m_Controllers[i];

	if( m_Debugger )
		delete m_Debugger;

	if( m_Renderer )
		delete m_Renderer;

	if( m_VarLoader )
		delete m_VarLoader;

	if( m_Entities )
		delete	m_Entities;

	for( int i=0; i < m_Cameras.size(); i++ )
		delete m_Cameras[i];
}