/* 
 * The generic vehicle entity.  AI and Player vehicles extend this class. 
 */ 
  
#include "Vehicle.h" 
#include "MessageManager.h"


//--------------------------------------------------------------------------------------
// Function:  update
//--------------------------------------------------------------------------------------
void Vehicle::update( Vec3 newPosition, Vec3 velocity, Matrix mat )
{
	Matrix translationMat;
	BoundingBox tempBB = getBoundingBox();

	//save previous position for speed calculation
	m_vPreviousPosition = m_vPosition;

	D3DXMatrixIdentity( &translationMat );
	D3DXMatrixTranslation( &translationMat, 0, -tempBB.m_fHeight, 0 );
	D3DXMatrixMultiply( &mat, &mat, &translationMat );

	setVelocity(velocity);
	Entity::update( newPosition, mat );

	float s = getSpeed();
	if (reversing) {
		m_Wheels[ 0 ].update( mat, -s ); 
		m_Wheels[ 1 ].update( mat, -s ); 
		m_Wheels[ 2 ].update( mat, -s ); 
		m_Wheels[ 3 ].update( mat, -s );
	}
	else {
		m_Wheels[ 0 ].update( mat, s ); 
		m_Wheels[ 1 ].update( mat, s ); 
		m_Wheels[ 2 ].update( mat, s ); 
		m_Wheels[ 3 ].update( mat, s );
	}

	m_Wheels[WHEEL0].setWheelLateral(NxVec3(1, 0, 0));
	m_Wheels[WHEEL1].setWheelLateral(NxVec3(-1, 0, 0));
	m_Wheels[WHEEL2].setWheelLateral(NxVec3(1, 0, 0));
	m_Wheels[WHEEL3].setWheelLateral(NxVec3(-1, 0, 0));
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Vehicle::initialize( Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename )
{
	//load mesh for vehicle
	Entity::initialize( device, pos, filename, effectFilename );

	//initialize speed to zero
	m_fSpeed = 0.0;
	m_fRunningTime = 0.0;
	m_fRunningDistance = 0.0;

	BoundingBox BB = this->getBoundingBox();
	float width = BB.m_fWidth/2;
	float length = BB.m_fLength/2;
	float height = -BB.m_fHeight/2;

	//load the wheel meshes for the vehicle
	m_Wheels[ WHEEL0 ].initialize( device, WHEEL0_FILE, Vec3(-2.5, height, 3.15), effectFilename);
	m_Wheels[ WHEEL1 ].initialize( device, WHEEL1_FILE, Vec3(2.5, height, 3.15), effectFilename );
	m_Wheels[ WHEEL2 ].initialize( device, WHEEL2_FILE, Vec3(2.5, height, -3.6), effectFilename );
	m_Wheels[ WHEEL3 ].initialize( device, WHEEL3_FILE, Vec3(-2.5, height, -3.6), effectFilename );

	reversing = false;
}


Vec3 Vehicle::lastPassedWP ()
{
	if (mind != 0)
		return mind->getLastPassedWaypoint (m_vPosition);
	else
		return Vec3 (0, 0, 0);
}

Vec3 Vehicle::nextWP ()
{
	if (mind != 0)
		return mind->getNextWaypoint (m_vPosition);
	else
		return Vec3 (0, 0, 0);
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
// Function:  setPlayerNum (Player 1 = 0, Player 2 = 1...etc)
//--------------------------------------------------------------------------------------
void Vehicle::setPlayerNum( int num )
{
	m_iPlayerNumber = num;
	mind->setPlayerNum( num );
}

//--------------------------------------------------------------------------------------
// Function:  getMass
//--------------------------------------------------------------------------------------
int Vehicle::getMass()
{
	return mass;
}


//--------------------------------------------------------------------------------------
// Function:  getSpeed
// This function calculates and returns the vehicle speed based on the last simulation.
//--------------------------------------------------------------------------------------
float Vehicle::getSpeed( )
{
	return (float)sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
}


//--------------------------------------------------------------------------------------
// Function:  setTimeElapsed
//--------------------------------------------------------------------------------------
void  Vehicle::setTimeElapsed( float time )
{
	m_fTimeElapsed = time;
	m_fRunningTime += time;
	mind->timeElapsed (time);
}


//--------------------------------------------------------------------------------------
// Function:  getInputObj
// Returns a pointer to the input object for this vehicle.
//--------------------------------------------------------------------------------------
Input* Vehicle::getInputObj ()
{
	return &input;
}


//--------------------------------------------------------------------------------------
// Function:  getDirection
// Returns a unit vector (in the XZ plane) in the direction the front of the car is
// facing.
//--------------------------------------------------------------------------------------

Vec3 Vehicle::getDirection ()
{
	Vec3 direction (0, 0, 1);
	D3DXVec3TransformNormal (&direction, &direction, &m_matWorld);
	direction.y = 0.0f;
	D3DXVec3Normalize (&direction, &direction);
	return direction;
}


//--------------------------------------------------------------------------------------
// Function:  getAI
//--------------------------------------------------------------------------------------

AI* Vehicle::getAI ()
{
	return mind;
}

bool Vehicle::isReversing()
{
	return reversing;
}

void Vehicle::setReverse(bool r)
{
	reversing = r;
}

int Vehicle::getRank()
{
	return m_iRank;
}

void Vehicle::setRank(int rank)
{
	m_iRank = rank;
}

bool Vehicle::isPlayer()
{
	return m_bIsPlayer;
}