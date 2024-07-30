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
	vec3& point();
	vec3& normal();
};

