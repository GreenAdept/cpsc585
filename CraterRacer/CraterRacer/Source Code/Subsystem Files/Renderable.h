#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H


//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "..\Utility Files\Constants.h"


//--------------------------------------------------------
//		CLASS: Renderable
//--------------------------------------------------------

class Renderable 
{

public:

	//Public Interface -----------------------------------

	Renderable( void ) { m_pMesh = NULL; }
	Renderable( const Renderable& renderableCopy ) ;
	~Renderable( );

	Renderable*	getUpdatedRenderable	( Vec3& position );
	HRESULT		computeMeshWorldMatrix	( LPD3DXMESH mesh, Matrix& mMeshWorld );

	void		releaseMemory			( );
	void		lostDevice				( );
	void		resetDevice				( Device* device );


	//Date Members ---------------------------------------

	// All data members are public for now, may change in the future

	bool			 m_bCanBeRendered;
	Vec3			 m_vPosition;
	Mesh*			 m_pMesh;
	LPCWSTR			 m_Filename;
	
	Effect*			 m_pEffect;
	Matrix			 m_matWorld;
	Handle			 m_hRenderObj;         // Handle to RenderObj technique
	Device*			 m_pDevice;
};

#endif // RENDERABLE_H