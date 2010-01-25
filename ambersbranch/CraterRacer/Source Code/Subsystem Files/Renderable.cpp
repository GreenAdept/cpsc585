/*
* The Renderable object.  Each entity has a renderable object.
* The rendering system takes the renderable object from each entity and displays it.
*/

#include "Renderable.h"

//--------------------------------------------------------------------------------------
// Constructor
//
// Right now we construct a temporary renderable object outside and pass 
// it into a newly constructed renderable to initialize it.  This is so we don't have to
// change the constructor all of the time when adding/removing required members that
// need to be determined from the outside.
//
// In the future we want to read most of the intialization from a file.
//--------------------------------------------------------------------------------------
Renderable::Renderable( const Renderable& renderableCopy  ) 
{
	// Initialize
	m_pDevice		 = renderableCopy.m_pDevice;
	m_Filename		 = renderableCopy.m_Filename;
	m_vPosition		 = renderableCopy.m_vPosition;
	m_bCanBeRendered = renderableCopy.m_bCanBeRendered;

	// Load the mesh
	m_pMesh = new Mesh( m_Filename );
	m_pMesh->Create( m_pDevice, m_Filename );

	// Create vertex/pixel shader effect from file
    D3DXCreateEffectFromFile( m_pDevice, OBJ_EFFECT, NULL, NULL, 0,
                                        NULL, &m_pEffect, NULL);

	m_hRenderObj = m_pEffect->GetTechniqueByName( "RenderScene" );

	// Compute the objects world matrix and bounding box
	//D3DXMatrixIdentity( &m_matWorld );
	computeMeshWorldMatrix( m_pMesh->GetMesh(), m_matWorld );
}


//--------------------------------------------------------------------------------------
// Compute the translate and scale transform for the current mesh.
// This function is taken from the samples in the DirectX Framework just to get it 
// (but we will put our own in soon!)
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
// Function:  getUpdatedRenderable
// Updates the renderable's position then returns itself so it is ready to be drawn 
// by the renderer.
//--------------------------------------------------------------------------------------
Renderable* Renderable::getUpdatedRenderable( Vec3& pos )
{
	m_vPosition = pos;
	return this;
}


//--------------------------------------------------------------------------------------
// Function: releaseMemory
// Releases any of the memory used by anything attached to its device.
//--------------------------------------------------------------------------------------
void Renderable::releaseMemory( )
{
	SAFE_RELEASE( m_pEffect );
}


//--------------------------------------------------------------------------------------
// Function: lostDevice
// Makes sure its members that are attached to its device are aware the 
// device is lost.
//--------------------------------------------------------------------------------------
void Renderable::lostDevice( )
{
	if( m_pEffect )
        m_pEffect->OnLostDevice();

	// mesh has to be destroyed since its residing device is lost
    m_pMesh->Destroy( );
}


//--------------------------------------------------------------------------------------
// Function: resetDevice
// Resets anything attached to its device.
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
Renderable::~Renderable( )
{
	if( m_pMesh )
		delete m_pMesh;
}