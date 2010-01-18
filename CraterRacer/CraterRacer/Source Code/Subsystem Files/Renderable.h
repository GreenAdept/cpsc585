#pragma once
#ifndef H_RENDERABLE
#define H_RENDERABLE

#include "..\..\Constants.h"


class Renderable {
	public:
		Renderable(void);

		bool	canBeRendered;
		Vec3	position;
};

#endif