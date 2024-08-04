#include "Plane.h"

Plane::Plane() : _point({ 0.0f, 0.0f, 0.0f }), _normal({ 0.0f, 0.0f, 0.0f }) {};
Plane::Plane(const vec3& point, const vec3& normal) : _point(point), _normal(normal) {}

Plane::Plane(const vec3& point, const float a, const float b, const float c, const float d) : _point(point) {
	_normal = normalize(vec3(a, b, c));
}

vec3& Plane::point() {
	return _point;
}
vec3& Plane::normal() {
	return _normal;
}