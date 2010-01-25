#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <vector>
#include "Renderable.h"
#include "..\Utility Files\Constants.h"

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

	Renderer ( );
	~Renderer( ) {}

	void render		( Device* device, vector<Renderable*>, MCamera& );
	void renderFloor( );

private:

	//Date Members ---------------------------------------
};

#endif