#include "Frustum.h"

Frustum::Frustum() {}

Frustum::Frustum(Camera& camera, float aspectRatio, float nearDistance, float farDistance) {
	vec3 nearPoint = camera.getForward() * nearDistance;
	vec3 farPoint = camera.getForward() * farDistance;
	const float halfFarQuadHeight = tanf(camera.fov()* 0.5f) * farDistance;
	const float halfFarQuadWidth = halfFarQuadHeight / aspectRatio;
	_near = Plane(camera.getPos() + nearPoint, camera.getForward());
	_far = Plane(camera.getPos() + farPoint, -camera.getForward());
	_top = Plane(camera.getPos(), normalize(cross(farPoint - (camera.getUp() * halfFarQuadHeight), camera.getRight())));
	_bottom = Plane(camera.getPos(), normalize(cross(camera.getRight(), farPoint + (camera.getUp() * halfFarQuadHeight))));
	_left = Plane(camera.getPos(), normalize(cross(farPoint + (camera.getRight() * halfFarQuadWidth), camera.getUp())));
	_right = Plane(camera.getPos(), normalize(cross(camera.getUp(), farPoint - (camera.getRight() * halfFarQuadWidth))));
	//_top = Plane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	//_bottom = Plane({ 0.0f, (float)(700 - 1), 0.0f }, { 0.0f, -1.0f, 0.0f });
	//_left = Plane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	//_right = Plane({ (float)(1200 - 1), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });
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
