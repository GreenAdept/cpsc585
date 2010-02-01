/* 
 * The generic vehicle entity.  AI and Player vehicles extend this class. 
 */ 
  
#include "Vehicle.h" 

void Vehicle::update(Vec3 newPosition, Vec3 velocity, Matrix mat)
{
	setVelocity(velocity);
	Entity::update(newPosition, mat);
}

void Vehicle::setVelocity(Vec3 velocity)
{
	this->velocity = velocity;
}

Vec3 Vehicle::getVelocity()
{
	return velocity;
}