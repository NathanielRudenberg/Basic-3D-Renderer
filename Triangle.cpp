#include "triangle.h"
#include <iostream>

Triangle::Triangle(RowVector4f& v1, RowVector4f& v2, RowVector4f& v3) {
	luminance = 0;
	v.push_back(v1); v.push_back(v2); v.push_back(v3);
}

Triangle::Triangle(const Triangle& old) {
	luminance = old.luminance;
	v = std::vector<RowVector4f>(old.v);
}

Triangle::Triangle(Triangle&& old) noexcept : luminance(std::move(old.luminance)), v(std::move(old.v)) { old.luminance = 0; }

std::vector<RowVector4f>& Triangle::getVerts() {
	return v;
}

int Triangle::getLuminance() {
	return luminance;
}

void Triangle::setLuminance(int lum) {
	luminance = lum;
}