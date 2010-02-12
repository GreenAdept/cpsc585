#include "Wheel.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the wheel object
//--------------------------------------------------------------------------------------
Wheel::Wheel() 
{
	//m_mMaterial = new NxMaterial();
	m_rRestitution		= NxReal(0.5);
	m_rStaticFriction	= NxReal(1.0);
	m_rDynamicFriction	= NxReal(0.3);

	m_fAngle			= 0; //degrees
	//m_fDiameter			= 0.5; //meters

	/*m_mMaterial->setDynamicFriction(m_rDynamicFriction);
	m_mMaterial->setStaticFriction(m_rStaticFriction);
	m_mMaterial->setRestitution(m_rRestitution);*/
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Wheel::initialize( Device* device, LPWSTR filename, Vec3 pt )
{
	Entity::initialize( device, Vec3(0,0,0), filename );
	
	Renderable* tempRenderable = this->getRenderable();
	m_fDiameter = 2 * tempRenderable->getBoundingBox().m_fRadius;
	m_fDisplacement = 0.0;
	setChassisPt( pt );
}


void Wheel::update( Matrix mat )
{
	Matrix m;
	Matrix translate;
	Vec3 sus = getSuspensionAxis() * m_fDisplacement;

	D3DXMatrixIdentity( &translate );
	D3DXMatrixTranslation( &translate, sus.x, sus.y, sus.z );
	D3DXMatrixMultiply( &m, &mat, &translate );

	Entity::update( m );
}


//--------------------------------------------------------------------------------------
// Function:  Destructor
//--------------------------------------------------------------------------------------
Wheel::~Wheel() 
{
	//delete m_mMaterial; Doesn't work - someone know why?
}

float Wheel::getDiameter()
{
	return m_fDiameter;
}

float Wheel::getAngle()
{
	return m_fAngle;
}

float Wheel::getDisplacement()
{
	return m_fDisplacement;
}

void Wheel::setDisplacement( float displacement )
{
	m_fDisplacement = displacement;
}

NxVec3 Wheel::getChassisPt()
{
	return m_vChassisPt;
}

void Wheel::setChassisPt( Vec3 pt )
{
	m_vChassisPt = NxVec3( pt.x, pt.y, pt.z );
}

float Wheel::getWheelWidth( )
{
	return getBoundingBox().m_fWidth;
}

Vec3 Wheel::getSuspensionAxis()
{
	return Vec3(m_vSuspensionAxis.x, m_vSuspensionAxis.y, m_vSuspensionAxis.z );
}

void Wheel::setSuspensionAxis( NxVec3 sus )
{
	m_vSuspensionAxis = sus;
}
