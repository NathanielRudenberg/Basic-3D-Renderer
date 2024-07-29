#pragma once
#include <utility>
#include <Eigen/StdVector>
#include <Eigen/Core>

using Eigen::RowVector4f;

class Triangle {
private:
	RowVector4f v[3];
	int luminance;

public:
	RowVector4f * getVerts();
	int getLuminance();
	void setLuminance(int lum);

public:
	// Constructors
	Triangle();
	Triangle(const RowVector4f& v1, const RowVector4f& v2, const RowVector4f& v3);
};