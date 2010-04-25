#pragma once
#ifndef ENTITY_H
#define ENTITY_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#define  NOMINMAX
#include "DXUT.h"
#include "NxPhysics.h"
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
protected: 
	//Accessible to all inheriting classes ---------------------

	NxActor*		m_nxActor;
	Renderable*		m_pRenderable;
	Vec3			m_vPosition;
	Matrix			m_matWorld;

public:
	//Public interface -----------------------------------------

	Entity ( ) { m_pRenderable = 0; m_nxActor = 0; }
	virtual ~Entity( );
	
	// Accessors
	Renderable*		getRenderable	  ( );
	BoundingBox		getBoundingBox	  ( );
	Vec3&			getPosition		  ( ) { return m_vPosition; }
	Matrix			getPositionMatrix ( ) { return m_matWorld; }

	virtual void	initialize		  ( Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename );

	virtual AI*		getAI			  ( ) { return 0; }
	NxActor*		getPhysicsObj	  ( ) { return m_nxActor; }
	void			setPhysicsObj	  ( NxActor* actor ) { m_nxActor = actor; }

	void			update			  ( Vec3 newPostion, Matrix & mat );
	void            update            ( Vec3 newPosition );
	void			update			  ( Matrix & matWorldTransform );
};

#endif //ENTITY_H