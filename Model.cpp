#include "model.h"

Model::Model() : objectMesh(Mesh()) {}

Model::Model(std::string filename) {
	objectMesh.loadObj(filename);
	_position << 0.0f, 0.0f, 0.0f;
}

Model::Model(std::string filename, float x, float y, float z) {
	objectMesh.loadObj(filename);
	_position << x, y, z;
}

void Model::load(std::string filename) {
	objectMesh.loadObj(filename);
}

Mesh& Model::getMesh() {
	return objectMesh;
}

void Model::moveTo(float x, float y, float z) {
	_position << x, y, z;
}

RowVector3f& Model::getPosition()
{
	return _position;
}
