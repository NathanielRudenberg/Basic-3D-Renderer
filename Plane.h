#pragma once
#include <Eigen/Core>

using Eigen::RowVector3f;


class Plane {
private:
	RowVector3f _point;
	RowVector3f _normal;

public:
	Plane();
	Plane(const RowVector3f& point, const RowVector3f& normal);
	RowVector3f& point();
	RowVector3f& normal();
};

