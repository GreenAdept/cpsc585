#pragma once
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

//-------------------------------------------------
// Include files
//-------------------------------------------------

#include "Constants.h"
#include "Entity.h"


//-------------------------------------------------
// Parameter to control the amount of frame delay
// in the camera.
//-------------------------------------------------

#define FRAME_DELAY 60


//-------------------------------------------------
// Class: GameCamera

// Implementation of a laggy game camera.
//-------------------------------------------------

class GameCamera {
	Entity *target;
	MCamera camera;

	Vec3 offset;
	Vec3 eye, lookAt;
	Vec3 interpolate (float distance, Vec3 newEye);
	
	int index;
	float distTotal;
	float distBuffer [FRAME_DELAY];

public:
	GameCamera();

	void setTarget (Entity *e);
	void updateWindow (const D3DSURFACE_DESC* pSurface);
	MCamera getCamera ();
};


#endif
