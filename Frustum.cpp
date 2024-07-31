#include "Frustum.h"

Frustum::Frustum(Camera& camera, float aspectRatio, float fov, float nearDistance, float farDistance) {
	vec3 nearPoint = camera.getForward() * nearDistance;
	vec3 farPoint = camera.getForward() * farDistance;
	const float halfFarQuadWidth = farDistance * tanf(fov * 0.5f);
	const float halfFarQuadHeight = halfFarQuadWidth * aspectRatio;
	_near = Plane(camera.getPos() + nearPoint, camera.getForward());
	_far = Plane(camera.getPos() + farPoint, -camera.getForward());
	_top = Plane(camera.getPos(), cross(camera.getRight(), farPoint - (camera.getUp() * halfFarQuadHeight)));
	_bottom = Plane(camera.getPos(), cross(farPoint + (camera.getUp() * halfFarQuadHeight), camera.getRight()));
	_left = Plane(camera.getPos(), cross(camera.getUp(), farPoint + (camera.getRight() * halfFarQuadWidth)));
	_right = Plane(camera.getPos(), cross(farPoint - (camera.getRight() * halfFarQuadWidth), camera.getUp()));
}

Plane& Frustum::near() {
	return _near;
}

Plane& Frustum::far() {
	return _far;
}

Plane& Frustum::top() {
	return _top;
}

Plane& Frustum::bottom() {
	return _bottom;
}

Plane& Frustum::left() {
	return _left;
}

Plane& Frustum::right() {
	return _right;
}
