/*
The Crater entity.
*/

#include "Crater.h"
#include <math.h>


void Crater::setUpVector (Vec3 up) {
	up /= D3DXVec3Length (&up);
	Vec3 defaultUp (0, 1, 0);
	Vec3 axis;

	D3DXVec3Cross (&axis, &defaultUp, &up);
	float sinA = D3DXVec3Length (&axis);
	axis /= sinA;

	Matrix m1, m2;
	D3DXMatrixRotationAxis (&m1, &axis, asin (sinA));
	D3DXMatrixTranslation (&m2, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixMultiply (&m1, &m1, &m2);
	update (m1);

	if (m_nxActor != 0) {
		NxMat33 nxmat (NxVec3 (m1(0,0), m1(1,0), m1(2,0)),
					   NxVec3 (m1(0,1), m1(1,1), m1(2,1)),
					   NxVec3 (m1(0,2), m1(1,2), m1(2,2)));
		m_nxActor->setGlobalOrientation (nxmat);
	}
}
