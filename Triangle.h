#pragma once
#include <utility>
#include <Eigen/StdVector>
#include <Eigen/Core>

//using Eigen::RowVectorXf;
using Eigen::RowVector4f;

class Triangle {
private:
	int luminance;

public:
	std::vector<RowVector4f> v;
	std::vector<RowVector4f>& getVerts();
	int getLuminance();
	void setLuminance(int lum);

public:
	// Constructors
	Triangle(RowVector4f& v1, RowVector4f& v2, RowVector4f& v3);
	Triangle(const Triangle& old);
	Triangle(Triangle&& old) noexcept;
};