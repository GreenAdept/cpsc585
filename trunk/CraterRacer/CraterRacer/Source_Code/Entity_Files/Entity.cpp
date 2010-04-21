/*
* The main entity class.  All entities extend this class.
* Each entity has a renderable object that can be displayed by the Renderer.
*/

#include "Entity.h"


//--------------------------------------------------------------------------------------
// Function:  Destructor 
//--------------------------------------------------------------------------------------
Entity::~Entity( ) 
{
	if( m_pRenderable )
		delete m_pRenderable;
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Entity::initialize( Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename )
{
	Renderable tempRenderable;
	tempRenderable.m_bCanBeRendered = true;
	tempRenderable.m_Filename = filename;
	tempRenderable.m_pDevice = device;
	tempRenderable.m_vPosition = pos;
	tempRenderable.m_EffectFilename = effectFilename;

	m_pRenderable = new Renderable( tempRenderable );
	m_vPosition = pos;
	m_matWorld = m_pRenderable->m_matWorld;
}


//--------------------------------------------------------------------------------------
// Function:  getRenderable
// Returns the entity's renderable.
//--------------------------------------------------------------------------------------
Renderable* Entity::getRenderable( )
{
	m_pRenderable->m_vPosition = m_vPosition;
	m_matWorld = m_pRenderable->m_matWorld;
	return m_pRenderable->getRenderable( );
}


//--------------------------------------------------------------------------------------
// Function:  getBoundingBox
// Returns the entity's renderable.
//--------------------------------------------------------------------------------------
BoundingBox Entity::getBoundingBox( )
{
	return m_pRenderable->getBoundingBox( );
}


//--------------------------------------------------------------------------------------
// Function:  update
// Update the entity's new position in the game.
//--------------------------------------------------------------------------------------
void Entity::update( Matrix matWorldTransform )
{
	m_pRenderable->m_matWorld = matWorldTransform;
	m_matWorld = matWorldTransform;
}

//--------------------------------------------------------------------------------------
// Function:  update
// Update the entity's new position in the game.
//--------------------------------------------------------------------------------------
void Entity::update( Vec3 newPosition )
{
	m_vPosition = newPosition;

	if (m_pRenderable != 0) {
		Matrix m;
		D3DXMatrixIdentity( &m );
		D3DXMatrixTranslation( &m, newPosition.x, newPosition.y, newPosition.z );

		update (m);
	}

	if (m_nxActor != 0)
		m_nxActor->setGlobalPosition (NxVec3 (newPosition.x, newPosition.y, newPosition.z));
}

//--------------------------------------------------------------------------------------
// Function:  update
// Update the entity's new position in the game.
//--------------------------------------------------------------------------------------
void Entity::update( Vec3 newPosition, Matrix matWorldTransform )
{
	m_vPosition = newPosition;

	m_pRenderable->m_matWorld = matWorldTransform;
	m_matWorld = matWorldTransform;
}
