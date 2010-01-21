#pragma once
#ifndef H_Fractal
#define H_Fractal

#include <iostream>
#include "DXUT.h"
#include "..\Utility Files\Constants.h"
#include "Renderable.h"

using namespace std;


class Entity {
	public:
		Entity( ) {}
		Entity( IDirect3DDevice9* device, const D3DSURFACE_DESC* ) ;
		~Entity( );
		
		Renderable* getRenderable();

		virtual void update(void);

protected: 
		//just inheriting classes can access these
		Renderable*		m_pRenderable;
		Vec3			m_vPosition;
};

#endif