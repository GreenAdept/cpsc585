#pragma once
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "..\Utility Files\Constants.h"
#include "Entity.h"

#define CAMERA_BUFFER_SIZE 8


class GameCamera {
	Entity *target;
	MCamera camera;
	int index;
	Vec3 buffer [CAMERA_BUFFER_SIZE];

public:
	GameCamera();

	void setTarget (Entity *e);
	void updateWindow (const D3DSURFACE_DESC* pSurface);
	MCamera getCamera ();
};


#endif
