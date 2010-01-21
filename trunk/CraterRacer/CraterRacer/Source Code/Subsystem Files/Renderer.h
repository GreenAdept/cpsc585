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
//--------------------------------------------------------

class Renderer 
{

public:

	//Public Interface -----------------------------------

	Renderer ( ) {}
	~Renderer( ) {}

	void render( Device* device, vector<Renderable*>, MCamera& );


private:

	//Date Members ---------------------------------------

};

#endif