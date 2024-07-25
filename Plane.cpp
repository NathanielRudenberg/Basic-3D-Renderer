#include "Plane.h"

Plane::Plane() : _point(RowVector3f{ 0.0f, 0.0f, 0.0f }), _normal(RowVector3f{ 0.0f, 0.0f, 0.0f }) {};
Plane::Plane(RowVector3f& point, RowVector3f& normal) : _point(point), _normal(normal) {}
Plane::Plane(RowVector3f point, RowVector3f normal) : _point(point), _normal(normal) {}

RowVector3f& Plane::point() {
	return _point;
}
RowVector3f& Plane::normal() {
	return _normal;
}