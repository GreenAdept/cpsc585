#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <vector>
#include "Renderable.h"
#include "GameCamera.h"
#include "Constants.h"

using namespace std;

//--------------------------------------------------------
//		CLASS: Renderer
//
//		This rendering subsystem handles all drawing to
//		the screen (device) for our game.  
//--------------------------------------------------------

class Renderer 
{

public:

	//Public Interface -----------------------------------

	Renderer ( ) {}
	~Renderer( ) {}

	void render		( Device* device, vector<Renderable*>, vector<GameCamera*> cameras );
	void renderFloor( );

private:

	//Date Members ---------------------------------------

};

#endif