/* 
 * The generic vehicle entity.  AI and Player vehicles extend this class. 
 */ 
  
#include "Vehicle.h" 


//--------------------------------------------------------------------------------------
// Function:  update
//--------------------------------------------------------------------------------------
void Vehicle::update( Vec3 newPosition, Vec3 velocity, Matrix mat )
{
	Matrix translationMat;
	BoundingBox tempBB = getBoundingBox();

	D3DXMatrixIdentity( &translationMat );
	D3DXMatrixTranslation( &translationMat, 0, -tempBB.m_fHeight, 0 );
	D3DXMatrixMultiply( &mat, &mat, &translationMat );

	setVelocity(velocity);
	Entity::update(newPosition, mat);

	m_Wheels[ 0 ].update( mat ); 
	m_Wheels[ 1 ].update( mat ); 
	m_Wheels[ 2 ].update( mat ); 
	m_Wheels[ 3 ].update( mat ); 

}


//--------------------------------------------------------------------------------------
// Function:  update
//--------------------------------------------------------------------------------------
void Vehicle::update( Vec3 newBodyPosition, Vec3 velocity, Matrix bodyMat, Matrix wheel0,
					 Matrix wheel1, Matrix wheel2, Matrix wheel3 )
{
	setVelocity( velocity );
	Entity::update( newBodyPosition, bodyMat );

	m_Wheels[ WHEEL0 ].update( wheel0 );
	m_Wheels[ WHEEL1 ].update( wheel1 );
	m_Wheels[ WHEEL2 ].update( wheel2 );
	m_Wheels[ WHEEL3 ].update( wheel3 );
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Vehicle::initialize( Device* device, Vec3 pos, LPCWSTR filename )
{
	Entity::initialize( device, pos, filename );

	m_Wheels[ WHEEL0 ].initialize( device, WHEEL0_FILE );
	m_Wheels[ WHEEL1 ].initialize( device, WHEEL1_FILE );
	m_Wheels[ WHEEL2 ].initialize( device, WHEEL2_FILE );
	m_Wheels[ WHEEL3 ].initialize( device, WHEEL3_FILE );
}


//--------------------------------------------------------------------------------------
// Function:  setVelocity
//--------------------------------------------------------------------------------------
void Vehicle::setVelocity( Vec3 velocity )
{
	this->velocity = velocity;
}


//--------------------------------------------------------------------------------------
// Function:  getVelocity
//--------------------------------------------------------------------------------------
Vec3 Vehicle::getVelocity( )
{
	return velocity;
}


//--------------------------------------------------------------------------------------
// Function:  setMass
//--------------------------------------------------------------------------------------
void Vehicle::setMass( int mass )
{
	this->mass = mass;
}


//--------------------------------------------------------------------------------------
// Function:  getMass
//--------------------------------------------------------------------------------------
int Vehicle::getMass()
{
	return mass;
}


Input* Vehicle::getInputObj ()
{
	return mind.getInputObj();
}