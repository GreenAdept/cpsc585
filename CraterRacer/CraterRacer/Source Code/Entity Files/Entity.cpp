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
// Temporarily initialize an object this way, but will eventually be reading this
// from file and initializing the entities from a data file.
//--------------------------------------------------------------------------------------
void Entity::initialize( Device* device, Vec3 pos, LPCWSTR filename )
{
	Renderable tempRenderable;
	tempRenderable.m_bCanBeRendered = true;
	tempRenderable.m_Filename = filename;
	tempRenderable.m_pDevice = device;
	tempRenderable.m_vPosition = pos;

	m_pRenderable = new Renderable( tempRenderable );
	m_vPosition = pos;
}


//--------------------------------------------------------------------------------------
// Function:  getRenderable
// Returns the entity's renderable.
//--------------------------------------------------------------------------------------
Renderable* Entity::getRenderable( )
{
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
void Entity::update( Vec3 newPosition, Matrix mat )
{
	m_vPosition = newPosition;

	m_pRenderable->m_matWorld = mat;
}


Vec3 Entity::getPosition() 
{
	return m_vPosition;
}