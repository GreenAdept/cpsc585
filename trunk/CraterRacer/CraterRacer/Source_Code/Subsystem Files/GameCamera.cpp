#include "GameCamera.h"



//------------------------------------------------------
// Constructor: GameCamera
// Creates a new GameCamera.
//------------------------------------------------------

GameCamera::GameCamera () {
	offset = Vec3 (0.0f, 5.0f, -20.0f);
	rearView = false;
	setTarget (0);
}


//------------------------------------------------------
// Private Function: interpolate
// Interpolates between the previous camera position
// and the ideal camera position to return the new
// camera position.
//------------------------------------------------------

Vec3 GameCamera::interpolate (float dist, Vec3 newEye) {
	float w = dist / (dist+5.0f);
	return eye*(1-w) + newEye*w;
}


//------------------------------------------------------
// Function: setTarget
// setTarget sets the entity that the camera will follow.
//------------------------------------------------------

void GameCamera::setTarget (Entity *e) {
	target = e;

	if (target==0) {
		lookAt = Vec3 (0.0f, 0.0f, 0.0f);
		eye = offset;
	}
	else {
		lookAt = target->getPosition();
		Vec3 temp;
		D3DXVec3TransformNormal (&temp, &offset, &target->getPositionMatrix());
		eye = lookAt + temp;
	}
}

//------------------------------------------------------
// Function: updateWindow
// Updates the window parameters.
//------------------------------------------------------

void GameCamera::updateWindow (const D3DSURFACE_DESC* pSurface) {
	float fAspectRatio = pSurface->Width / (float)pSurface->Height;
	camera.SetProjParams (D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	camera.SetWindow (pSurface->Width, pSurface->Height);
}


//------------------------------------------------------
// Function: getCamera
// getCamera returns a CModelViewerCamera with data on
// the game camera's current position.
//------------------------------------------------------

MCamera GameCamera::getCamera () {
	if (target == 0) {
		camera.SetViewParams (&eye, &lookAt);
		return camera;
	}

	Vec3 path = target->getPosition() - lookAt;
	float dist = D3DXVec3Length (&path);

	lookAt = target->getPosition();
	Vec3 temp;
	D3DXVec3TransformNormal (&temp, &offset, &target->getPositionMatrix());
	temp.y = offset.y;
	Vec3 newEye = lookAt + temp;

	eye = interpolate (dist, newEye);
	camera.SetViewParams (&eye, &lookAt);
	return camera;
}


//------------------------------------------------------
// Function: setRearView
// Activates/deactivates rear view camera mode.
//------------------------------------------------------

void GameCamera::setRearView (bool activate) {
	if (rearView == activate)
		return;
	else
		toggleRearView ();
}

//------------------------------------------------------
// Function: toggleRearView
// Toggles rear view camera mode on/off.
//------------------------------------------------------

void GameCamera::toggleRearView () {
	rearView = !rearView;
	offset.x = -offset.x;
	offset.z = -offset.z;

	Vec3 temp = eye - lookAt;
	temp.x = -temp.x;
	temp.z = -temp.z;
	eye = lookAt + temp;
}
