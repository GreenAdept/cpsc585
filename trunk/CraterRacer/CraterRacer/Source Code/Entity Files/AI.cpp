#include "AI.h"


float distSquared (Vec3 A, Vec3 B) {
	Vec3 C = B-A;
	return C.x*C.x + C.z*C.z;
}
