//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//--------------------------------------------------------------------------------------
#include "GameObj.h"

//--------------------------------------------------------------------------------------
// Function:  initGame
// Here we initialize the game's camera and scene.  Eventually we want to read all of this
// in via file IO.  
//--------------------------------------------------------------------------------------
void GameObj::initGame( IDirect3DDevice9* device, const D3DSURFACE_DESC* pSurface )
{
	m_Simulator = new Simulator();
	m_Controller1 = new XBox360Controller(0); //player 1 controller

	Vec3 pos( 0.0f, 10.0f, 0.0f );
	Vec3 terrainPos( 0.0f, 0.0f, 0.0f );
	
	// Create entities
	Vehicle *pv = m_Entities.makePlayer (device, pos, OBJ_FILE);
	//Meteor *m = m_Entities.makeMeteor (device, Vec3 (-10.0f, 0.0f, 0.0f), OBJ_FILE);

	// Create the terrain
	m_Terrain = new Terrain( );
	m_Terrain->initialize( device, terrainPos, TERRAIN_FILE );

	// Add our vehicle to the PhysX system
	m_Simulator->createVehicle( pos, pv->getBoundingBox() );

	// Initialize camera and set it to follow the player
	m_Camera.updateWindow( pSurface );
	m_Camera.setTarget( pv );    //comment out this line to make the camera stationary

	// Clear debug.txt
	debug.clearFile();
}


//--------------------------------------------------------------------------------------
// Function: addInput
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{
	vector<Vehicle*> m_Vehicles = m_Entities.getVehicles();
	Vehicle* v = m_Vehicles[0];
	/*
	switch (virtualKeyCode)
	{
	case VK_LEFT:
		//add left-ward force to player vehicle
		v->drive(Input::Arrow::LEFT, isKeyDown);
		break;
	case VK_UP:
		//add upward force to player vehicle
		v->drive(Input::Arrow::UP, isKeyDown);
		break;
	case VK_RIGHT:
		//add right-ward force to player vehicle
		v->drive(Input::Arrow::RIGHT, isKeyDown);
		break;
	case VK_DOWN:
		//add downward force to player vehicle
		v->drive(Input::Arrow::DOWN, isKeyDown);
		break;
	default:
		break;
	}
	*/
	switch (virtualKeyCode)
	{
	case VK_LEFT:
		//add left-ward force to player vehicle
		v->setInput(Input::Arrow::LEFT, isKeyDown);
		break;
	case VK_UP:
		//add upward force to player vehicle
		v->setInput(Input::Arrow::UP, isKeyDown);
		break;
	case VK_RIGHT:
		//add right-ward force to player vehicle
		v->setInput(Input::Arrow::RIGHT, isKeyDown);
		break;
	case VK_DOWN:
		//add downward force to player vehicle
		v->setInput(Input::Arrow::DOWN, isKeyDown);
		break;
	default:
		break;
	}
}


//--------------------------------------------------------------------------------------
// Function: think
//--------------------------------------------------------------------------------------
void GameObj::think ()
{
	vector<AI*> minds = m_Entities.getAIs (METEORS);
	for (int i=0; i<minds.size(); i++) {
		minds[i]->think (&m_Entities, METEORS, i);
		if (minds[i]->getState() == AI::TRIGGERED)
			debug.writeToFile ("Triggered.");
	}
}


//--------------------------------------------------------------------------------------
// Function: processInput
//--------------------------------------------------------------------------------------
void GameObj::processInput( float fElapsedTime )
{
	vector<Vehicle*> m_Vehicles = m_Entities.getVehicles();
	m_Controller1->Update(fElapsedTime);
	Vehicle* v = m_Vehicles[0];
	Vec3 dir(m_Controller1->LeftThumbstick.GetX(), 0, m_Controller1->LeftThumbstick.GetY());
	/*
	if (m_Controller1->A.WasPressedOrHeld())
	{
		//accelerate
		drive *= 3; //3 for the acceleration right now, need to CHANGE!
	}
	else if (m_Controller1->B.WasPressedOrHeld())
	{
		Vec3 vlc = v->getVelocity();
		double speed = sqrt(vlc.x * vlc.x + vlc.y * vlc.y + vlc.z * vlc.z);
		//if speed > 0.1km/h, decelerate
		//if ( speed > 0.1)
		{
			//give a force opposite current direction, add on joystick direction
			Vec3 oppositeForce = (-1) * vlc; //the -1 needs to be CHANGED to a braking constant
			drive = oppositeForce + drive * 0.2 * speed;
		}
		//else reverse -> while reversing, turning is backwards
	}
	v->setDir(drive);
	*/
	
	if (m_Controller1->A.WasPressedOrHeld())
	{
		v->drive(dir, true, false);
	}
	else if (m_Controller1->B.WasPressedOrHeld())
	{
		v->drive(dir, false, true);
	}
	else
	{
		v->drive(dir, false, false);
	}
}


//--------------------------------------------------------------------------------------
// Function: render
// This function renders the game's scene to the specified device.
//--------------------------------------------------------------------------------------
void GameObj::render( Device* device )
{
	vector<Renderable*> renderables = m_Entities.getRenderables();
	renderables.push_back( m_Terrain->getRenderable() );

	// pass the renderables off to the renderer to do all the work
	m_Renderer->render( device, renderables, m_Camera.getCamera() );
}


//--------------------------------------------------------------------------------------
// Function: simulate
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	m_Simulator->simulate( m_Entities.getVehicles(), fElapsedTime );
	//debug.writeToFile("game obj");
	//debug.writeToFile( m_Vehicles[0]->getPosition());
}


//--------------------------------------------------------------------------------------
// Function: processCallback
// This function is used to manage game objects that are attached to a device when the
// device is reset, lost or destroyed.  
//--------------------------------------------------------------------------------------
void GameObj::processCallback( ProcessType type, Device* device, const D3DSURFACE_DESC* pBackSurface )
{
	vector<Entity*> m_Vehicles = m_Entities.getEntities();

	// when the device is destroyed, we want to release all of the memory attached to it
	if( type == ON_DESTROY )
	{
		// make sure the renderables are aware their device is being destroyed, so they
		// can release the appropriate memory
		for( unsigned int i = 0; i < m_Vehicles.size(); i++ ) 
			 m_Vehicles[i]->getRenderable( )->releaseMemory( );

		if( m_Terrain )
			m_Terrain->getRenderable( )->releaseMemory( );
	}

	// when the device is lost, we want to prepare some of objects for destruction or reset
	else if( type == ON_LOST )
	{
		// make sure the renderables are aware their device is lost
		for( unsigned int i = 0; i < m_Vehicles.size(); i++ ) 
			 m_Vehicles[i]->getRenderable( )->lostDevice( );

		if( m_Terrain )
			m_Terrain->getRenderable( )->lostDevice( );
	}

	// when the device is reset the dimensions might have changed, so we want to make sure
	// anything based on the dimensions are adjusted
	else if( type == ON_RESET )
	{
		// re-adjust the camera's projection parameters
		m_Camera.updateWindow (pBackSurface);

		// let the renderables know their device was reset
		for( unsigned int i = 0; i < m_Vehicles.size(); i++ ) 
			 m_Vehicles[i]->getRenderable( )->resetDevice( device );

		if( m_Terrain )
			m_Terrain->getRenderable( )->resetDevice( device );
	}
}


//--------------------------------------------------------------------------------------
// Function:  Destructor 
//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	if( m_Simulator )
		delete m_Simulator;

	if( m_Terrain )
		delete m_Terrain;

	if( m_Controller1 )
		delete m_Controller1;
}