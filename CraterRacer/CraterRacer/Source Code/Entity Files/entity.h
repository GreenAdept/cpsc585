#pragma once
#ifndef H_Fractal
#define H_Fractal

#include <iostream>
#include "DXUT.h"
#include "renderable.h"

using namespace std;
typedef D3DXVECTOR3 Vec3;


class Entity {
	public:
		Entity(void);
		Renderable getRenderable() { return renderable; }

		virtual void update(void);

private:
		Renderable renderable;
		Vec3	   position;

};

#endif