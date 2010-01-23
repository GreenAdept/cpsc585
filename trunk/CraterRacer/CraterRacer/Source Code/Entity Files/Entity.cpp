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
// Function:  Constructor
// Initializes renderable based on device input.
//--------------------------------------------------------------------------------------
Entity::Entity( Device* device ) 
{
	// Temporarily initialize an object, but will eventually be reading this
	// from file and initializing the entities from file data.
	Renderable tempRenderable;
	tempRenderable.m_bCanBeRendered = true;
	tempRenderable.m_Filename = OBJ_FILE;
	tempRenderable.m_pDevice = device;
	tempRenderable.m_vPosition = Vec3(0.0,0.0,0.0);

	m_pRenderable = new Renderable( tempRenderable );
	m_vPosition = Vec3( 0.0f, 0.0f, 0.0f );
}


//--------------------------------------------------------------------------------------
// Function:  getRenderable
// Returns the entity's renderable.
//--------------------------------------------------------------------------------------
Renderable* Entity::getRenderable( )
{
	return m_pRenderable->getUpdatedRenderable( m_vPosition );
}

void Entity::update( Vec3 newPosition )
{
	m_vPosition = newPosition;
}

void Entity::setInput(Input::Arrow dir, bool isKeyDown)
{

}
