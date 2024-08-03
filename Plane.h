#pragma once
#include <glm.hpp>

using glm::vec3;


class Plane {
private:
	vec3 _point;
	vec3 _normal;

public:
	Plane();
	Plane(const vec3& point, const vec3& normal);
	Plane(const vec3& point, const float a, const float b, const float c, const float d);
	vec3& point();
	vec3& normal();
};

