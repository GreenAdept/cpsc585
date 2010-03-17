#pragma once
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

//-------------------------------------------------
// Include files
//-------------------------------------------------

#include "Constants.h"
#include "Entity.h"


//-------------------------------------------------
// Class: GameCamera

// Implementation of a laggy game camera.
//-------------------------------------------------

class GameCamera {
	Entity *target;
	MCamera camera;

	bool rearView;
	Vec3 offset;
	Vec3 eye, lookAt;
	Vec3 interpolate (float distance, Vec3 newEye);

public:
	GameCamera();

	void    setTarget      (Entity *e);
	void    updateWindow   (const D3DSURFACE_DESC* pSurface);
	MCamera getCamera      ();
	void    setRearView    (bool activate);
	void    toggleRearView ();
};


#endif
