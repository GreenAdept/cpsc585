/*
The Rendering system.
The rendering system takes the renderable object from each entity and displays it.
*/

#include "Renderer.h"

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void Renderer::render( Device* device, vector<Renderable*> renderables, MCamera& g_Camera ) 
{
	HRESULT		hr;
	Matrix		mWorld,
				mView,
				mProj,
				mWorldViewProjection,
				mMeshWorld;
	LPD3DXMESH	tempMesh;
	Mesh		*dxMesh;
	Renderable	*tempR;
	Effect		*pEffect;

	// sort renderables here...perhaps by type and then take out the hidden objects

	// now go through and render each one
	for( unsigned int i = 0; i < renderables.size(); i++ )
	{
		tempR = renderables[i];
		dxMesh = tempR->m_pMesh;
		tempMesh = dxMesh->GetMesh();
		pEffect = tempR->m_pEffect;
		mMeshWorld = tempR->m_matWorld;
   
		// Get the projection & view matrix from the camera class
		mWorld = *g_Camera.GetWorldMatrix();
		mProj = *g_Camera.GetProjMatrix();
		mView = *g_Camera.GetViewMatrix();

		mWorldViewProjection = mMeshWorld * mWorld * mView * mProj;
		pEffect->SetTechnique( tempR->m_hRenderObj );

		// Update the effect's variables.  Instead of using strings, it would 
		// be more efficient to cache a handle to the parameter by calling 
		// ID3DXEffect::GetParameterByName
		V( pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( pEffect->SetMatrix( "g_mWorld", &mWorld ) );

		UINT cPasses;
        V( pEffect->Begin( &cPasses, 0 ) );
        for( UINT p = 0; p < cPasses; ++p )
        {
            V( pEffect->BeginPass( p ) );

			// set and draw each of the materials in the mesh
			for( UINT i = 0; i < dxMesh->m_dwNumMaterials; i++ )
			{
				V( pEffect->SetVector( "g_vDiffuse", ( Vec4* )&dxMesh->m_pMaterials[i].Diffuse ) );

				V( pEffect->SetTexture( "g_txScene", dxMesh->m_pTextures[i] ) );

				V( pEffect->CommitChanges() );

				tempMesh->DrawSubset( i );
			}

			V( pEffect->EndPass() );
		}
        V( pEffect->End() );
	}
}