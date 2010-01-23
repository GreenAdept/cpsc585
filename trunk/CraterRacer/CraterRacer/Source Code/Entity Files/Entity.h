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
#include "Input.h"


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
	virtual void	update( Vec3 newPosition );

	void setInput(Input::Arrow dir, bool isKeyDown);

private:
	Vec3			m_vPosition;

protected: 
	
	//Accessible to all inheriting classes ---------------------

	Renderable*		m_pRenderable; 


};

#endif //ENTITY_H