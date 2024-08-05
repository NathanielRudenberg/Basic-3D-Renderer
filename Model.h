#pragma once
#include "mesh.h"
#include <fstream>
#include <glm.hpp>
#include <iostream>
using glm::vec3;

class Model {
private:
	Mesh objectMesh;
	vec3 _position;

public:
	Model();
	Model(std::string filename);
	Model(std::string filename, float x, float y, float z);

	// Load model from an obj file.
	//
	// @param filename - The path to the obj file.
	void load(std::string filename);

	// Get the model's mesh object.
	//
	// @return The internal triangle mesh of the model.
	Mesh& getMesh();

	// Move the model to a new position within the world space.
	void moveTo(const vec3& position);

	// Move the model to a new position within the world space.
	void moveTo(float x, float y, float z);

	vec3& getPosition();
};