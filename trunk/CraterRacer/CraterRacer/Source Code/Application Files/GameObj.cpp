//--------------------------------------------------------------------------------------
// File: GameObj.cpp
//
//--------------------------------------------------------------------------------------
#include "GameObj.h"


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
GameObj::GameObj()
{

}


void GameObj::addInput( UINT uMsg, WPARAM wParam )
{


}


void GameObj::processInput( )
{

}


void GameObj::render( )
{
	vector<Renderable> renderables(entities.size());
	
	for (unsigned int i = 0; i < entities.size(); i++) {
		renderables[i] = entities[i]->getRenderable();
	}

	renderer.render(renderables);

}

void GameObj::simulate( float fElapsedTime )
{
	//simulator.simulate(entities);
}