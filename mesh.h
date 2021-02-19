#pragma once
#define _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING
#include <fstream>
#include <strstream>
#include "triangle.h"
#include <iostream>

class Mesh {
private:
	enum coords {
		X,
		Y,
		Z,
		W
	};

	std::vector<Triangle> tris;

public:
	bool loadObj(std::string filename);
	std::vector<Triangle>& getTris();
};
