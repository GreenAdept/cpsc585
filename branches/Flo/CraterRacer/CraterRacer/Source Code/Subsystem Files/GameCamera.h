#pragma once
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

//-------------------------------------------------
// Include files
//-------------------------------------------------

#include "Constants.h"
#include "Entity.h"

//-------------------------------------------------
// Parameter to control the size of the camera
// position buffer. Larger values mean a more laggy
// camera.
//-------------------------------------------------

#define CAMERA_BUFFER_SIZE 8


//-------------------------------------------------
// Class: GameCamera

// Implementation of a laggy game camera.
//-------------------------------------------------

class GameCamera {
	Entity *target;
	MCamera camera;

	int index;
	Vec3 offset;
	Vec3 buffer [CAMERA_BUFFER_SIZE];

public:
	GameCamera();

	void setTarget (Entity *e);
	void updateWindow (const D3DSURFACE_DESC* pSurface);
	MCamera getCamera ();
};


#endif
