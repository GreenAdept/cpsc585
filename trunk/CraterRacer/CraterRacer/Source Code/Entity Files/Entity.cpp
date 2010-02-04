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
	m_matWorld = m_pRenderable->m_matWorld;
}


//--------------------------------------------------------------------------------------
// Function:  getRenderable
// Returns the entity's renderable.
//--------------------------------------------------------------------------------------
Renderable* Entity::getRenderable( )
{
	//m_pRenderable->m_vPosition = m_vPosition;
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
void Entity::update( Vec3 newPosition, Matrix matWorldTransform )
{
	m_vPosition = newPosition;

	m_pRenderable->m_matWorld = matWorldTransform;
	m_matWorld = matWorldTransform;
}


//--------------------------------------------------------------------------------------
// Function:  getPosition
// Returns the vector position of the entity
//--------------------------------------------------------------------------------------
Vec3 Entity::getPosition( ) 
{
	return m_vPosition;
}


//--------------------------------------------------------------------------------------
// Function:  getPositionMatrix
// Returns the world transformation matrix for the entity
//--------------------------------------------------------------------------------------
Matrix Entity::getPositionMatrix( )
{
	return m_matWorld;
}