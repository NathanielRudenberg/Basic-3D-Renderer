#pragma once
#include <utility>
#include <glm.hpp>

using glm::vec4;

class Triangle {
private:
	vec4 v[3];
	int luminance;

public:
	vec4* getVerts();
	int getLuminance();
	void setLuminance(int lum);

public:
	// Constructors
	Triangle();
	Triangle(const vec4& v1, const vec4& v2, const vec4& v3);
};