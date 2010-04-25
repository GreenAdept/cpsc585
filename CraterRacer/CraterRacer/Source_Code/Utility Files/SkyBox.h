#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Constants.h"
#include "GameCamera.h"
#include <string>


//--------------------------------------------------------
//		CLASS: SkyBox
//
//		This class represents a spherical sky that is 
//		textured with stars that can be rendered surrounding
//		a camera position.
//--------------------------------------------------------
class SkyBox
{

public:

	//Public Interface -----------------------------------

	SkyBox( ) { }
	~SkyBox( );

	void		init			( Device* device, float skyRadius );
	void		OnLostDevice	( );
	void		OnResetDevice	( );
	void		renderSkyBox	( MCamera* camera );

private:

	//Data Members ---------------------------------------

	LPD3DXMESH	m_SphereMesh;		//hemisphere sky
	float		m_fRadius;			//radius of skybox
	ICubeTex*	m_EnvironmentMap;	//.dds star texture
	Effect*		m_Effect;			//effect to render sky
};

#endif // SKYBOX_H