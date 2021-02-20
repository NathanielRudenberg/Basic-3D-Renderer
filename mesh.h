#pragma once
#include <fstream>
#include <sstream>
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
