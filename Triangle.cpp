#include "triangle.h"
#include <iostream>

Triangle::Triangle() {
	luminance = 0;
}

Triangle::Triangle(RowVector4f& v1, RowVector4f& v2, RowVector4f& v3) {
	luminance = 0;
	v[0] = v1; v[1] = v2; v[2] = v3;
}

RowVector4f * Triangle::getVerts() {
	return v;
}

int Triangle::getLuminance() {
	return luminance;
}

void Triangle::setLuminance(int lum) {
	luminance = lum;
}