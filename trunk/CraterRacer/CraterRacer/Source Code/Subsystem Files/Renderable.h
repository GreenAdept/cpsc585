#pragma once
#ifndef H_RENDERABLE
#define H_RENDERABLE

#include "DXUT.h"
typedef D3DXVECTOR3 Vec3;


class Renderable {
	public:
		Renderable(void);

		bool	canBeRendered;
		Vec3	position;
};

#endif