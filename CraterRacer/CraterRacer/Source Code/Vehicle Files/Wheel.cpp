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
void Wheel::initialize( Device* device, LPWSTR filename )
{
	Entity::initialize( device, Vec3(0,0,0), filename );
	
	Renderable* tempRenderable = this->getRenderable();
	m_fDiameter = 2 * tempRenderable->getBoundingBox().m_fRadius;
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