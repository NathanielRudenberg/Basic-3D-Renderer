#pragma once
#define _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING
#include <fstream>
#include <iostream>
#include "mesh.h"

class Model {
private:
	Mesh objectMesh;

public:
	Model();
	void load(std::string filename);
	Mesh& getMesh();
};