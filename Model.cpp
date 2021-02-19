#include "model.h"

Model::Model() : objectMesh(Mesh()) {}

void Model::load(std::string filename) {
	objectMesh.loadObj(filename);
}

Mesh& Model::getMesh() {
	return objectMesh;
}