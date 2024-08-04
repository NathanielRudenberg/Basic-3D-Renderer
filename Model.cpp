#include "model.h"

Model::Model() : objectMesh(Mesh()) {}

Model::Model(std::string filename) {
	objectMesh.loadObj(filename);
	_position = vec3(0.0f, 0.0f, 0.0f);
}

Model::Model(std::string filename, float x, float y, float z) {
	objectMesh.loadObj(filename);
	_position =  vec3(x, y, z);
}

void Model::load(std::string filename) {
	objectMesh.loadObj(filename);
}

Mesh& Model::getMesh() {
	return objectMesh;
}

void Model::moveTo(const vec3& position) {
	_position = position;
}

void Model::moveTo(float x, float y, float z) {
	_position = vec3(x, y, z);
}

vec3& Model::getPosition()
{
	return _position;
}
