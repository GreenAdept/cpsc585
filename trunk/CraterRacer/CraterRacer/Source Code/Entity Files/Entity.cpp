#include "entity.h"

Entity::Entity() 
{
	renderable.canBeRendered = true;
	position = Vec3( 0.0f, 0.0f, 0.0f );
}

void Entity::update() {}