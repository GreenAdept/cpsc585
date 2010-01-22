//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//--------------------------------------------------------------------------------------
#include "GameObj.h"


//--------------------------------------------------------------------------------------
GameObj::GameObj( )
{
   
}


//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	for (unsigned int i = 0; i < m_Entities.size(); i++) 
	{
		// delete all entities to ensure no memory leaks occur
		if( m_Entities[i] )
			delete m_Entities[i];
	}
}


//--------------------------------------------------------------------------------------
// Function:  initGame
// Here we initialize the game's camera and scene.  Eventually we want to read all of this
// in via file IO.  
//--------------------------------------------------------------------------------------
void GameObj::initGame( IDirect3DDevice9* device, const D3DSURFACE_DESC* pSurface )
{
	//init camera to new device, with perspective view
	float fAspectRatio = pSurface->Width / ( FLOAT )pSurface->Height;
	m_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
	m_Camera.SetWindow( pSurface->Width, pSurface->Height );
	
	// Setup the camera's view parameters
    Vec3 vecEye( 0.0f, 0.0f, -5.0f );
    Vec3 vecAt ( 0.0f, 0.0f, -0.0f );
    m_Camera.SetViewParams( &vecEye, &vecAt );

	//testing...adding one entity/object to the list, first make sure the list is empty
	for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
		delete m_Entities[i];

	m_Entities.clear();
	m_Entities.push_back( new Entity( device, pSurface ) );
}


//--------------------------------------------------------------------------------------
// Function: addInput
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{


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
	vector<Renderable*> renderables( m_Entities.size() );
	
	// Render all the entities by retrieving their renderable components
	for (unsigned int i = 0; i < m_Entities.size(); i++) {
		renderables[i] = m_Entities[i]->getRenderable( );
	}

	// pass the renderables off to the renderer to do all the work
	m_Renderer->render( device, renderables, m_Camera );
}


//--------------------------------------------------------------------------------------
// Function: simulate
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	//simulator.simulate(entities);
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
	}

	// when the device is lost, we want to prepare some of objects for destruction or reset
	else if( type == ON_LOST )
	{
		// make sure the renderables are aware their device is lost
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->lostDevice( );
	}

	// when the device is reset the dimensions might have changed, so we want to make sure
	// anything based on the dimensions are adjusted
	else if( type == ON_RESET )
	{
		// re-adjust the camera's projection parameters
		float fAspectRatio = pBackSurface->Width / ( FLOAT )pBackSurface->Height;
		m_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
		m_Camera.SetWindow( pBackSurface->Width, pBackSurface->Height );

		// let the renderables know their device was reset
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->resetDevice( device );
	}
}
