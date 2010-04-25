#include "SkyBox.h"


//--------------------------------------------------------------------------------------
// Function: init
// This function initializes the skybox by creating a sky sphere mesh and
// it's rendering effect.
//--------------------------------------------------------------------------------------
void SkyBox::init( Device* device, float skyRadius )
{
	HRESULT hr;
	m_fRadius = skyRadius;
	
	D3DXCreateSphere( device, skyRadius, 30, 30, &m_SphereMesh, 0 );
	D3DXCreateCubeTextureFromFile( device, L"Media\\Images\\EMap.dds", &m_EnvironmentMap );
	
	//load the effect that will draw the skybox
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    D3DXCreateEffectFromFile( device, L"Source_Code\\Effect_Files\\SkyBox.fx", NULL, NULL, dwShaderFlags,
                                        NULL, &m_Effect, NULL );

	// Set effect parameters that do not vary.
	m_Effect->SetTechnique( "SkyTechnique" );
	m_Effect->SetTexture( "g_EnvironmentMap", m_EnvironmentMap );
}


//--------------------------------------------------------------------------------------
// Function: DECONSTRUCTOR
// This function releases all resources attached to a device.
//--------------------------------------------------------------------------------------
SkyBox::~SkyBox( )
{
	if( m_SphereMesh )
		m_SphereMesh->Release( );
	
	if( m_EnvironmentMap )
		m_EnvironmentMap->Release( );

	if( m_Effect )
		m_Effect->Release( );
}


//--------------------------------------------------------------------------------------
// Function: OnLostDevice
// This function tells the effect the device has been lost
//--------------------------------------------------------------------------------------
void SkyBox::OnLostDevice( )
{
	m_Effect->OnLostDevice( );
}


//--------------------------------------------------------------------------------------
// Function: OnResetDevice
// This function resets the effect
//--------------------------------------------------------------------------------------
void SkyBox::OnResetDevice( )
{
	m_Effect->OnResetDevice( );
}


//--------------------------------------------------------------------------------------
// Function: renderSkyBox
// This function draws the skybox surrounding the specified camera position
//--------------------------------------------------------------------------------------
void SkyBox::renderSkyBox( MCamera* camera )
{
	// SkyBox always centered about camera's position.
	m_Effect->SetMatrix( "g_WorldViewProj", &( *camera->GetWorldMatrix() * (*camera->GetViewMatrix()) * (*camera->GetProjMatrix()) ));

	UINT numPasses = 0;

	m_Effect->Begin( &numPasses, 0 );
	m_Effect->BeginPass( 0 );

	m_SphereMesh->DrawSubset(0);
	
	m_Effect->EndPass( );
	m_Effect->End( );
}