#pragma once
#ifndef H_RENDERER
#define H_RENDERER

#include <vector>
#include "renderable.h"


using namespace std;

class Renderer {
	public:
		Renderer(void);

		void render(vector<Renderable> test);
};

#endif