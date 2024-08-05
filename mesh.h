#pragma once
#include "triangle.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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
