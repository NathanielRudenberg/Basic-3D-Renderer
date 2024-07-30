#include "triangle.h"
#include <iostream>

Triangle::Triangle() {
	luminance = 0;
}

Triangle::Triangle(const vec4& v1, const vec4& v2, const vec4& v3) {
	luminance = 0;
	v[0] = v1; v[1] = v2; v[2] = v3;
}

vec4* Triangle::getVerts() {
	return v;
}

int Triangle::getLuminance() {
	return luminance;
}

void Triangle::setLuminance(int lum) {
	luminance = lum;
}