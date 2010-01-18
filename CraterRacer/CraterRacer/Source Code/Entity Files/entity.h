#pragma once
#ifndef H_Fractal
#define H_Fractal

#include <iostream>
#include "DXUT.h"
#include "..\..\Constants.h"
#include "renderable.h"

using namespace std;


class Entity {
	public:
		Entity(void);
		Renderable getRenderable() { return renderable; }

		virtual void update(void);

protected: 
		//just inheriting classes can access these
		Renderable renderable;
		Vec3	   position;

};

#endif