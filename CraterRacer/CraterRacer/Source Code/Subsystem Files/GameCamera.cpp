#include "GameCamera.h"



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
		Renderable *r = target->getRenderable();
		for (index=0; index<CAMERA_BUFFER_SIZE; index++)
			buffer[index] = offset + r->m_vPosition;
	}

	index = 0;
}

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
		Renderable *r = target->getRenderable();

		//Calculates and sets camera position
		buffer[index] = offset + r->m_vPosition;
		index = (index+1) % CAMERA_BUFFER_SIZE;
		camera.SetViewParams( buffer+index, &(r->m_vPosition) );
	}
	return camera;
}
