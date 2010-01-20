#include "renderable.h"

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
Renderable::Renderable( const Renderable& renderableCopy  ) 
{
	// Initialize
	m_pDevice		= renderableCopy.m_pDevice;
	m_Filename		= renderableCopy.m_Filename;
	m_vPosition		= renderableCopy.m_vPosition;
	m_bCanBeRendered = renderableCopy.m_bCanBeRendered;

	m_pMesh = new Mesh( m_Filename );
	m_pMesh->Create( m_pDevice, m_Filename );

	// Create vertex/pixel shader effect from file
    D3DXCreateEffectFromFile( m_pDevice, OBJ_EFFECT, NULL, NULL, 0,
                                        NULL, &m_pEffect, NULL);

	m_hRenderObj = m_pEffect->GetTechniqueByName( "RenderScene" );

	computeMeshWorldMatrix( m_pMesh->GetMesh(), m_matWorld );
}


//--------------------------------------------------------------------------------------
// Compute the translate and scale transform for the current mesh.
// This function is taken from the samples in the DirectX Framework just to get it 
// (but we will put our own in when we get it working..)
//--------------------------------------------------------------------------------------
HRESULT Renderable::computeMeshWorldMatrix( LPD3DXMESH pMesh, Matrix& mMeshWorld )
{
	D3DXMatrixIdentity( &mMeshWorld );

    LPDIRECT3DVERTEXBUFFER9 pVB;
    if( FAILED( pMesh->GetVertexBuffer( &pVB ) ) )
        return E_FAIL;

    LPVOID pVBData;
    if( SUCCEEDED( pVB->Lock( 0, 0, &pVBData, D3DLOCK_READONLY ) ) )
    {
        D3DXVECTOR3 vCtr;
        float fRadius;
        D3DXComputeBoundingSphere( ( D3DXVECTOR3* )pVBData, pMesh->GetNumVertices(),
                                   D3DXGetFVFVertexSize( pMesh->GetFVF() ),
                                   &vCtr, &fRadius );

        D3DXMatrixTranslation( &mMeshWorld, -vCtr.x, -vCtr.y, -vCtr.z );
        Matrix m;
        D3DXMatrixScaling( &m, 1 / fRadius, 1 / fRadius, 1 / fRadius );
        D3DXMatrixMultiply( &mMeshWorld, &mMeshWorld, &m );

        pVB->Unlock();
    }

    SAFE_RELEASE( pVB );

    return S_OK;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
Renderable* Renderable::getUpdatedRenderable( Vec3& pos )
{
	m_vPosition = pos;

	return this;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void Renderable::releaseMemory( )
{
	SAFE_RELEASE( m_pEffect );
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void Renderable::lostDevice( )
{
	if( m_pEffect )
        m_pEffect->OnLostDevice();

    m_pMesh->Destroy( );
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void Renderable::resetDevice( Device* device )
{
	if( m_pEffect )
		m_pEffect->OnResetDevice( );

	if( m_pMesh )
	{
		m_pMesh->Destroy( );
		m_pMesh->Create( device, m_Filename );
	}
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
Renderable::~Renderable( )
{
	if( m_pMesh )
		delete m_pMesh;
}