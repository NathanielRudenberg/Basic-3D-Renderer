#include "triangle.h"
#include <iostream>

Triangle::Triangle(RowVector4f& v1, RowVector4f& v2, RowVector4f& v3) {
	luminance = 0;
	std::clog << "Regular constructor" << std::endl;
	v.push_back(v1); v.push_back(v3); v.push_back(v3);
	std::clog << "First tri vert: " << v[0] << std::endl;
	/*v[0] = v1;
	v[1] = v2;
	v[2] = v3;*/
}

Triangle::Triangle(const Triangle& old) {
	std::clog << "Copy constructor" << std::endl;
	luminance = old.luminance;
	v = std::vector<RowVector4f>(old.v);
}

Triangle::Triangle(Triangle&& old) noexcept : luminance(std::move(old.luminance)), v(std::move(old.v)) { std::clog << "Move constructor" << std::endl; }

std::vector<RowVector4f>& Triangle::getVerts() {
	//std::clog << "Getting vertex: " << v.at(0) << std::endl;
	return v;
}

int Triangle::getLuminance() {
	return luminance;
}

void Triangle::setLuminance(int lum) {
	luminance = lum;
}