#pragma once
#ifndef ENTITY_H
#define ENTITY_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "DXUT.h"
#include "Constants.h"
#include "Renderable.h"
#include "Input.h"
#include "AI.h"


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
	~Entity( );
		
	// Accessors
	Renderable*		getRenderable	  ( );
	BoundingBox		getBoundingBox	  ( );
	Vec3&			getPosition		  ( );
	Matrix			getPositionMatrix ( );

	virtual void	initialize		  ( Device* device, Vec3 pos, LPCWSTR filename );

	virtual AI*		getAI			  ( ) { return 0; }

	void			update			  ( Vec3 newPostion, Matrix mat );
	void			update			  ( Matrix matWorldTransform );

protected: 
	
	//Accessible to all inheriting classes ---------------------

	Renderable*		m_pRenderable;
	Vec3			m_vPosition;
	Matrix			m_matWorld;

};

#endif //ENTITY_H