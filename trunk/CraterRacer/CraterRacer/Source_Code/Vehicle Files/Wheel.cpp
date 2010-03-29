#include "Wheel.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the wheel object
//--------------------------------------------------------------------------------------
Wheel::Wheel() 
{
	m_rRestitution		= NxReal(0.5);
	m_rStaticFriction	= NxReal(1.0);
	m_rDynamicFriction	= NxReal(0.3);

	m_fAngle			= 0; //degrees
	m_fSpinAngle		= 0;
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Wheel::initialize( Device* device, LPCWSTR filename, Vec3 pt, LPCWSTR effectFilename )
{
	Entity::initialize( device, Vec3(0,0,0), filename, effectFilename );
	
	Renderable* tempRenderable = this->getRenderable();
	m_fDiameter = 2 * tempRenderable->getBoundingBox().m_fRadius;
	m_fDisplacement = 0.0;
	setChassisPt( pt );
}


//--------------------------------------------------------------------------------------
// Function:  initialize
//--------------------------------------------------------------------------------------
void Wheel::update( Matrix mat, float speed )
{
	Matrix m1, m2;
	Matrix translate, rotate;

	Vec3 sus = getSuspensionAxis() * m_fDisplacement;
	float yModifier = m_vChassisPt.y + (m_fDiameter/2 + 0.35);
	float tempAngle = m_fAngle;
	if (tempAngle >= 180)
		tempAngle -= 180;

	m_fSpinAngle += speed / 3.14;
	D3DXMatrixRotationX( &m1, m_fSpinAngle*(D3DX_PI/180));
	D3DXMatrixRotationY( &m2, -tempAngle*(D3DX_PI/180));
	D3DXMatrixMultiply( &rotate, &m1, &m2 );

	D3DXMatrixTranslation( &translate, -m_vChassisPt.x, -yModifier, -m_vChassisPt.z );
	D3DXMatrixMultiply( &m1, &translate, &rotate );

	D3DXMatrixTranslation( &translate, m_vChassisPt.x + sus.x, yModifier + sus.y, m_vChassisPt.z + sus.z );
	D3DXMatrixMultiply( &m1, &m1, &translate );
	D3DXMatrixMultiply( &m1, &m1, &mat );


	Entity::update( m1 );
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

void Wheel::setAngle(float angle) {
	m_fAngle = angle;
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

NxVec3 Wheel::getGlobalChassisPt()
{
	return m_vGlobalChassisPt;
}

void Wheel::setGlobalChassisPt( Vec3 pt )
{
	m_vGlobalChassisPt = NxVec3( pt.x, pt.y, pt.z );
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

void Wheel::setWheelLateral(NxVec3 lateral) {
	m_vWheelLateral = lateral;
}

NxVec3 Wheel::getWheelLateral() {
	return m_vWheelLateral;
}