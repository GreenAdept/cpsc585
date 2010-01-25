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

	//init camera to new device, with perspective view
	m_Camera.updateWindow( pSurface );

	//testing...adding one entity/object to the list, first make sure the list is empty
	for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
		delete m_Entities[i];
	PlayerVehicle *pv = new PlayerVehicle( device, OBJ_FILE );
	
	//add this new entity to simulator subsystem so it can be managed by PhysX
	Vec3 pos( 0.0f, 0.0f, 0.0f );
	m_Simulator->addActor( pv->getRenderable()->m_pMesh, pos );

	m_Entities.clear();
	m_Entities.push_back( pv );
	m_Camera.setTarget( pv );

	m_Terrain = new Terrain( device, TERRAIN_FILE );
}


//--------------------------------------------------------------------------------------
// Function: addInput
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{
	Entity* e = m_Entities[0];
	switch (virtualKeyCode)
	{
	case VK_LEFT:
		//add left-ward force to player vehicle
		e->setInput(Input::Arrow::LEFT, isKeyDown);
		break;
	case VK_UP:
		//add upward force to player vehicle
		e->setInput(Input::Arrow::UP, isKeyDown);
		break;
	case VK_RIGHT:
		//add right-ward force to player vehicle
		e->setInput(Input::Arrow::RIGHT, isKeyDown);
		break;
	case VK_DOWN:
		//add downward force to player vehicle
		e->setInput(Input::Arrow::DOWN, isKeyDown);
		break;
	default:
		break;
	}
}


//--------------------------------------------------------------------------------------
// Function: processInput
//--------------------------------------------------------------------------------------
void GameObj::processInput( )
{

}


//--------------------------------------------------------------------------------------
// Function: render
// This function renders the game's scene to the specified device.
//--------------------------------------------------------------------------------------
void GameObj::render( Device* device )
{
	vector<Renderable*> renderables;//( m_Entities.size() );
	
	// Render all the entities by retrieving their renderable components
	/*for (unsigned int i = 0; i < m_Entities.size(); i++) {
		renderables[i] = m_Entities[i]->getRenderable( );
	}*/
	
	renderables.push_back( m_Terrain->getRenderable( ) );
	//m_Renderer->renderFloor( );

	// pass the renderables off to the renderer to do all the work
	m_Renderer->render( device, renderables, m_Camera.getCamera() );
}


//--------------------------------------------------------------------------------------
// Function: simulate
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	m_Simulator->simulate( m_Entities, fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Function: processCallback
// This function is used to manage game objects that are attached to a device when the
// device is reset, lost or destroyed.  
//--------------------------------------------------------------------------------------
void GameObj::processCallback( ProcessType type, Device* device, const D3DSURFACE_DESC* pBackSurface )
{
	// when the device is destroyed, we want to release all of the memory attached to it
	if( type == ON_DESTROY )
	{
		// make sure the renderables are aware their device is being destroyed, so they
		// can release the appropriate memory
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->releaseMemory( );

		m_Terrain->getRenderable( )->releaseMemory( );
	}

	// when the device is lost, we want to prepare some of objects for destruction or reset
	else if( type == ON_LOST )
	{
		// make sure the renderables are aware their device is lost
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->lostDevice( );
		m_Terrain->getRenderable( )->lostDevice( );
	}

	// when the device is reset the dimensions might have changed, so we want to make sure
	// anything based on the dimensions are adjusted
	else if( type == ON_RESET )
	{
		// re-adjust the camera's projection parameters
		m_Camera.updateWindow (pBackSurface);

		// let the renderables know their device was reset
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->resetDevice( device );
		
		m_Terrain->getRenderable( )->resetDevice( device );
	}
}


//--------------------------------------------------------------------------------------
// Function:  Destructor 
//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	for (unsigned int i = 0; i < m_Entities.size(); i++) 
	{
		// delete all entities to ensure no memory leaks occur
		if( m_Entities[i] )
			delete m_Entities[i];
	}

	if( m_Simulator )
		delete m_Simulator;

	if( m_Terrain )
		delete m_Terrain;
}