#include "GameCamera.h"


//------------------------------------------------------
// Constructor: GameCamera
// Creates a new GameCamera.
//------------------------------------------------------

GameCamera::GameCamera () {
	offset = Vec3 (0.0f, 5.0f, -25.0f);
	setTarget (0);
}


//------------------------------------------------------
// Function: setTarget
// setTarget sets the entity that the camera will follow.
//------------------------------------------------------

void GameCamera::setTarget (Entity *e) {
	target = e;

	if (target==0) {
		for (index=0; index<CAMERA_BUFFER_SIZE; index++)
			buffer[index] = Vec3 (0.0f, 0.0f, 0.0f);
	}
	else {
		Vec3 position = target->getPosition();
		for (index=0; index<CAMERA_BUFFER_SIZE; index++)
			buffer[index] = offset + position;
	}

	index = 0;
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
	if (target==0) {
		camera.SetViewParams( &offset, buffer+index );
	}
	else {
		Vec3 position = target->getPosition();

		//Calculates and sets camera position
		buffer[index] = offset + position;
		index = (index+1) % CAMERA_BUFFER_SIZE;
		camera.SetViewParams( buffer+index, &position );
	}
	return camera;
}
