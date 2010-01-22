#pragma once
#ifndef ENTITY_H
#define ENTITY_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <iostream>
#include "DXUT.h"
#include "..\Utility Files\Constants.h"
#include "Renderable.h"

using namespace std;


//-----------------------------------------------------------------
//		CLASS: Entity
//
//		This is the base class of all the objects in our game.
//		An entity is not abstract yet, but should not be instantiated 
//		on its own.
//------------------------------------------------------------------

class Entity 
{
public:
		
	//Public interface -----------------------------------------

	Entity ( ) { }
	Entity ( Device* device );
	~Entity( );
		
	Renderable*		getRenderable( );
	virtual void	update( void ) {}

protected: 
	
	//Accessible to all inheriting classes ---------------------

	Renderable*		m_pRenderable; 
	Vec3			m_vPosition;

};

#endif //ENTITY_H