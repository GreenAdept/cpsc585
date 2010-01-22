/*
* The Rendering subsystem.
* The rendering system takes the renderable object from each entity and displays it.
*/

#include "Renderer.h"

//--------------------------------------------------------------------------------------
// Function: render
// Draws objects to the specified screen (device).  These renderables of these objects 
// are passed to this function and contain all the rendering-specific information needed
// by the renderer.  The latest camera is also required.
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
	Mesh		*dxMesh;	//the DXUT wrapper version of the mesh with helpful functions
	Renderable	*tempR;		//pointer to current renderable
	Effect		*pEffect;	//the effect currently being used


	// sort renderables here...perhaps by type and then take out the hidden objects


	// now go through and render each object via its renderable
	for( unsigned int i = 0; i < renderables.size(); i++ )
	{
		// initialize some temporary variables to make things easier
		tempR = renderables[i];
		dxMesh = tempR->m_pMesh;
		tempMesh = dxMesh->GetMesh();
		pEffect = tempR->m_pEffect;
		mMeshWorld = tempR->m_matWorld;
   
		// get the projection & view matrix from the camera class
		mWorld = *g_Camera.GetWorldMatrix();
		mProj = *g_Camera.GetProjMatrix();
		mView = *g_Camera.GetViewMatrix();

		// create the world view projection matrix
		mWorldViewProjection = mMeshWorld * mWorld * mView * mProj;

		// set the effect to be used
		pEffect->SetTechnique( tempR->m_hRenderObj );

		// Update the effect's variables.  The SDK examples say it would 
		// be more efficient to cache a handle to the parameter by calling 
		// ID3DXEffect::GetParameterByName, so we should eventually implement that
		V( pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( pEffect->SetMatrix( "g_mWorld", &mWorld ) );

		// start rendering
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