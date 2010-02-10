#include "GameCamera.h"



//------------------------------------------------------
// Constructor: GameCamera
// Creates a new GameCamera.
//------------------------------------------------------

GameCamera::GameCamera () {
	offset = Vec3 (0.0f, 5.0f, -20.0f);
	setTarget (0);
}


//------------------------------------------------------
// Private Function: interpolate
// Interpolates between the previous camera position
// and the ideal camera position to return the new
// camera position.
//------------------------------------------------------

Vec3 GameCamera::interpolate (float dist, Vec3 newEye) {
	float w = (dist+0.0001f) / (distTotal+dist+0.0001f);
	return eye*(1-w) + newEye*w;
}


//------------------------------------------------------
// Function: setTarget
// setTarget sets the entity that the camera will follow.
//------------------------------------------------------

void GameCamera::setTarget (Entity *e) {
	target = e;
	index = 0;
	distTotal = 0.0f;

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
	Vec3 newEye = lookAt + temp;
	
	if (dist <= 0.0001f) {
		if (index > 0) {
			index--;
			distTotal -= distBuffer[index];
			eye = interpolate (distBuffer[index], newEye);
		}
		else
			distTotal = 0.0f;
		camera.SetViewParams (&eye, &lookAt);
		return camera;
	}

	if (index < FRAME_DELAY) {
		distBuffer[index] = dist;
		distTotal += dist;
		index++;
		camera.SetViewParams (&eye, &lookAt);
		return camera;
	}
	else {
		eye = interpolate (dist, newEye);
		camera.SetViewParams (&eye, &lookAt);
		return camera;
	}
}
