// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
#pragma once
#include "Plane.h"
#include "camera.h"

class Frustum
{
	Plane _near;
	Plane _far;
	Plane _top;
	Plane _bottom;
	Plane _left;
	Plane _right;

public:
	Frustum(Camera& camera, float aspectRatio, float fov, float nearDistance, float farDistance);
	Plane& near();
	Plane& far();
	Plane& top();
	Plane& bottom();
	Plane& left();
	Plane& right();
};

