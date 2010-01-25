#include "GameCamera.h"



GameCamera::GameCamera () {
	index = 0;
}

void GameCamera::setTarget (Entity *e) {
	target = e;
	buffer[0] = Vec3 (0.0f, 0.0f, -5.0f);
	buffer[1] = Vec3 (0.0f, 0.0f, -5.1f);
	buffer[2] = Vec3 (0.0f, 0.0f, -5.2f);
	buffer[3] = Vec3 (0.0f, 0.0f, -5.3f);
	buffer[4] = Vec3 (0.0f, 0.0f, -5.4f);
	buffer[5] = Vec3 (0.0f, 0.0f, -5.3f);
	buffer[6] = Vec3 (0.0f, 0.0f, -5.2f);
	buffer[7] = Vec3 (0.0f, 0.0f, -5.1f);
}

void GameCamera::updateWindow (const D3DSURFACE_DESC* pSurface) {
	float fAspectRatio = pSurface->Width / (float)pSurface->Height;
	camera.SetProjParams (D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	camera.SetWindow (pSurface->Width, pSurface->Height);
}

MCamera GameCamera::getCamera () {
	Renderable *r = target->getRenderable();

	//Calculates and sets camera position
	Vec3 offset = Vec3 (0.0f, 0.0f, -5.0f);
	buffer[index] = offset + r->m_vPosition;

	index = (index+1) % CAMERA_BUFFER_SIZE;
	camera.SetViewParams( &buffer[index], &(r->m_vPosition) );
	return camera;
}
