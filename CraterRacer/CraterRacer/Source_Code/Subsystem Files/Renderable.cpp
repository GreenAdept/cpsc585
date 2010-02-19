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
	m_EffectFilename = renderableCopy.m_EffectFilename;
	m_vPosition		 = renderableCopy.m_vPosition;
	m_bCanBeRendered = renderableCopy.m_bCanBeRendered;

	// Load the mesh
	m_pMesh = new Mesh( m_Filename );
	m_pMesh->Create( m_pDevice, m_Filename );

	// Create vertex/pixel shader effect from file
    D3DXCreateEffectFromFile( m_pDevice, m_EffectFilename, NULL, NULL, 0,
                                        NULL, &m_pEffect, NULL);

	m_hRenderObj = m_pEffect->GetTechniqueByName( "RenderScene" );

	// Compute the objects world matrix and bounding box
	computeMeshWorldMatrix( m_vPosition	);
}


//--------------------------------------------------------------------------------------
// Compute the translate and scale transform for the current mesh.
// This function is taken from the samples in the DirectX Framework just to get it 
// (but we will put our own in soon!)
//--------------------------------------------------------------------------------------
HRESULT Renderable::computeMeshWorldMatrix( Vec3 startPos )
{
	D3DXMatrixIdentity( &m_matWorld  );
	Vec3 min, max;

	//make sure the mesh has vertex data first
    LPDIRECT3DVERTEXBUFFER9 pVB;
    if( FAILED( m_pMesh->GetMesh()->GetVertexBuffer( &pVB ) ) )
        return E_FAIL;

    LPVOID pVBData;
    if( SUCCEEDED( pVB->Lock( 0, 0, &pVBData, D3DLOCK_READONLY ) ) )
    {
        Vec3 vCtr;
		float radius = 0;
        D3DXComputeBoundingBox( ( D3DXVECTOR3* )pVBData, m_pMesh->m_dwNumVertices,
                                   D3DXGetFVFVertexSize( m_pMesh->GetMesh()->GetFVF() ),
                                   &min, &max );

		D3DXComputeBoundingSphere( ( D3DXVECTOR3* )pVBData, m_pMesh->m_dwNumVertices,
                       D3DXGetFVFVertexSize( m_pMesh->GetMesh()->GetFVF() ),
                       &vCtr, &radius );

		//translate the object to the position we want it to be in
		D3DXMatrixTranslation( &m_matWorld, startPos.x, startPos.y, startPos.z );

		//set the bounding box values for future reference
		m_BoundingBox.m_fHeight = max.y - min.y;
		m_BoundingBox.m_fWidth = max.x - min.x;
		m_BoundingBox.m_fLength = max.z - min.z;	
		m_BoundingBox.m_fRadius = 1.0;
		m_BoundingBox.m_fCenter = vCtr;

        pVB->Unlock();
    }

    SAFE_RELEASE( pVB );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Function:  getUpdatedRenderable
//--------------------------------------------------------------------------------------
Renderable* Renderable::getRenderable( )
{
	return this;
}


//--------------------------------------------------------------------------------------
// Function:  getUpdatedRenderable
//--------------------------------------------------------------------------------------
BoundingBox Renderable::getBoundingBox( )
{
	return m_BoundingBox;
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