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

Frustum::Frustum(Camera& camera, float aspectRatio, float nearDistance, float farDistance, const mat4& viewProjectionMatrix) {
	vec3 nearPoint = camera.getForward() * nearDistance;
	vec3 farPoint = camera.getForward() * farDistance;
	// column2 + column3
	float nearA = viewProjectionMatrix[2][0] + viewProjectionMatrix[3][0];
	float nearB = viewProjectionMatrix[2][1] + viewProjectionMatrix[3][1];
	float nearC = viewProjectionMatrix[2][2] + viewProjectionMatrix[3][2];
	float nearD = viewProjectionMatrix[2][3] + viewProjectionMatrix[3][3];
	_near = Plane(camera.getPos() + nearPoint, nearA, nearB, nearC, nearD);

	// column3 - column2
	float farA = -viewProjectionMatrix[2][0] + viewProjectionMatrix[3][0];
	float farB = -viewProjectionMatrix[2][1] + viewProjectionMatrix[3][1];
	float farC = -viewProjectionMatrix[2][2] + viewProjectionMatrix[3][2];
	float farD = -viewProjectionMatrix[2][3] + viewProjectionMatrix[3][3];
	_far = Plane(camera.getPos() + farPoint, farA, farB, farC, farD);

	// column3 - column1 
	float topA = -viewProjectionMatrix[1][0] + viewProjectionMatrix[3][0];
	float topB = -viewProjectionMatrix[1][1] + viewProjectionMatrix[3][1];
	float topC = -viewProjectionMatrix[1][2] + viewProjectionMatrix[3][2];
	float topD = -viewProjectionMatrix[1][3] + viewProjectionMatrix[3][3];
	_top = Plane(camera.getPos(), topA, topB, topC, topD);

	// column1 + column3
	float bottomA = viewProjectionMatrix[1][0] + viewProjectionMatrix[3][0];
	float bottomB = viewProjectionMatrix[1][1] + viewProjectionMatrix[3][1];
	float bottomC = viewProjectionMatrix[1][2] + viewProjectionMatrix[3][2];
	float bottomD = viewProjectionMatrix[1][3] + viewProjectionMatrix[3][3];
	_bottom = Plane(camera.getPos(), bottomA, bottomB, bottomC, bottomD);

	// column0 + column3
	float leftA = viewProjectionMatrix[0][0] + viewProjectionMatrix[3][0];
	float leftB = viewProjectionMatrix[0][1] + viewProjectionMatrix[3][1];
	float leftC = viewProjectionMatrix[0][2] + viewProjectionMatrix[3][2];
	float leftD = viewProjectionMatrix[0][3] + viewProjectionMatrix[3][3];
	_left = Plane(camera.getPos(), leftA, leftB, leftC, leftD);

	// column3 - column0
	float rightA = -viewProjectionMatrix[0][0] + viewProjectionMatrix[3][0];
	float rightB = -viewProjectionMatrix[0][1] + viewProjectionMatrix[3][1];
	float rightC = -viewProjectionMatrix[0][2] + viewProjectionMatrix[3][2];
	float rightD = -viewProjectionMatrix[0][3] + viewProjectionMatrix[3][3];
	_right = Plane(camera.getPos(), rightA, rightB, rightC, rightD);

	//const float halfFarQuadHeight = tanf(camera.fov()* 0.5f) * farDistance;
	//const float halfFarQuadWidth = halfFarQuadHeight / aspectRatio;
	//_near = Plane(camera.getPos() + nearPoint, camera.getForward());
	//_far = Plane(camera.getPos() + farPoint, -camera.getForward());
	//_top = Plane(camera.getPos(), normalize(cross(farPoint - (camera.getUp() * halfFarQuadHeight), camera.getRight())));
	//_bottom = Plane(camera.getPos(), normalize(cross(camera.getRight(), farPoint + (camera.getUp() * halfFarQuadHeight))));
	//_left = Plane(camera.getPos(), normalize(cross(farPoint + (camera.getRight() * halfFarQuadWidth), camera.getUp())));
	//_right = Plane(camera.getPos(), normalize(cross(camera.getUp(), farPoint - (camera.getRight() * halfFarQuadWidth))));
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
