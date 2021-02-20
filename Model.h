#pragma once
#include <fstream>
#include <iostream>
#include "mesh.h"

class Model {
private:
	Mesh objectMesh;

public:
	Model();

	// Load model from an obj file.
	//
	// @param filename - The path to the obj file.
	void load(std::string filename);

	// Get the model's mesh object.
	//
	// @return The internal triangle mesh of the model.
	Mesh& getMesh();

	// Move the model to a new position within the world space.
	void moveTo(float x, float y, float z);
};