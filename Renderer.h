#pragma once
#include <Eigen/Core>
#include "Triangle.h"
#include "camera.h"
#include "Plane.h"
#include "TransformUtilities.h"

using Eigen::RowVector3f;

class Renderer {
private:
	RowVector3f getTriangleNormal(Triangle& triTransformed);
	RowVector3f getCameraRay(RowVector3f& v);
	int getLuminance(RowVector3f& normal);
	void clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster);
};

