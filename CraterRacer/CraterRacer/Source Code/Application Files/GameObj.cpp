//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//
//--------------------------------------------------------------------------------------
#include "GameObj.h"


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
GameObj::GameObj( )
{
   
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
GameObj::~GameObj( )
{
	for (unsigned int i = 0; i < m_Entities.size(); i++) 
	{
		if( m_Entities[i] )
			delete m_Entities[i];
	}
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::initGame(  IDirect3DDevice9* device, const D3DSURFACE_DESC* pSurface )
{
	//init camera to new device, with perspective view
	float fAspectRatio = pSurface->Width / ( FLOAT )pSurface->Height;
	m_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
	m_Camera.SetWindow( pSurface->Width, pSurface->Height );
	
	// Setup the camera's view parameters
    Vec3 vecEye( 0.0f, 0.0f, -5.0f );
    Vec3 vecAt ( 0.0f, 0.0f, -0.0f );
    m_Camera.SetViewParams( &vecEye, &vecAt );

	//testing...adding one entity/object to the list
	for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
		delete m_Entities[i];

	m_Entities.clear();
	m_Entities.push_back( new Entity( device, pSurface ) );
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::addInput( bool isKeyDown, UINT virtualKeyCode )
{


}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::processInput( )
{

}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::render( Device* device )
{
	vector<Renderable*> renderables( m_Entities.size() );
	
	for (unsigned int i = 0; i < m_Entities.size(); i++) {
		renderables[i] = m_Entities[i]->getRenderable( );
	}

	m_Renderer->render( device, renderables, m_Camera );
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::simulate( float fElapsedTime )
{
	//simulator.simulate(entities);
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void GameObj::processCallback( ProcessType type, Device* device, const D3DSURFACE_DESC* pBackSurface )
{
	
	if( type == ON_DESTROY )
	{
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->releaseMemory( );
	}

	else if( type == ON_LOST )
	{
		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->lostDevice( );
	}

	else if( type == ON_RESET )
	{
		// Setup the camera's projection parameters
		float fAspectRatio = pBackSurface->Width / ( FLOAT )pBackSurface->Height;
		m_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
		m_Camera.SetWindow( pBackSurface->Width, pBackSurface->Height );

		for( unsigned int i = 0; i < m_Entities.size(); i++ ) 
			 m_Entities[i]->getRenderable( )->resetDevice( device );
	}
}
