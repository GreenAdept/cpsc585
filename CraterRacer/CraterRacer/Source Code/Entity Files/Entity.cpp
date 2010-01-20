#include "entity.h"

Entity::~Entity() 
{
	if( m_pRenderable )
		delete m_pRenderable;
}


Entity::Entity( IDirect3DDevice9* device, const D3DSURFACE_DESC* pSurface ) 
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

Renderable* Entity::getRenderable( )
{
	return m_pRenderable->getUpdatedRenderable( m_vPosition );
}


void Entity::update() {}