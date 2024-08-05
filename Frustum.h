// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
// https://gamedev.stackexchange.com/a/156758
#pragma once
#include "camera.h"
#include "Plane.h"

using glm::mat4;

class Frustum
{
	Plane _near;
	Plane _far;
	Plane _top;
	Plane _bottom;
	Plane _left;
	Plane _right;

public:
	Frustum();
	Frustum(Camera& camera, float aspectRatio, float nearDistance, float farDistance, const mat4& viewProjectionMatrix);
	Plane& near();
	Plane& far();
	Plane& top();
	Plane& bottom();
	Plane& left();
	Plane& right();
};