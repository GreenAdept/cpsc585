#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H


//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Constants.h" //typedefs and constants used in this class


//--------------------------------------------------------
//		CLASS: Renderable
//
//		This class contains all entity information needed
//		by the renderer.
//--------------------------------------------------------

class Renderable 
{

public:

	//Public Interface -----------------------------------

	Renderable( void ) { m_pMesh = NULL; }
	Renderable( const Renderable& renderableCopy ) ;
	~Renderable( );

	Renderable*	getRenderable			( );
	BoundingBox getBoundingBox			( );
	HRESULT		computeMeshWorldMatrix	( Vec3 startPos );

	void		releaseMemory			( );
	void		lostDevice				( );
	void		resetDevice				( Device* device );


	//Date Members ---------------------------------------

	// All data members are public for now, may change in the future

	bool			 m_bCanBeRendered;
	Vec3			 m_vPosition;
	Mesh*			 m_pMesh;
	LPCWSTR			 m_Filename;
	LPCWSTR			 m_EffectFilename;
	
	Effect*			 m_pEffect;
	Matrix			 m_matWorld;
	Handle			 m_hRenderObj;      // Handle to loaded effect
	Device*			 m_pDevice;			// the device attached to the mesh and effect
	BoundingBox		 m_BoundingBox;
};

#endif // RENDERABLE_H