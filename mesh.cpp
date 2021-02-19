#include "mesh.h"

bool Mesh::loadObj(std::string filename) {
	std::ifstream objFile(filename);
	if (!objFile.is_open()) { return false; }

	// Build vertex cache
	std::vector<RowVector4f> verts;
	while (!objFile.eof()) {
		char line[128];
		objFile.getline(line, 128);

		std::strstream s;
		s << line;

		char tmp;
		if (line[0] == 'v') {
			RowVector4f v;
			float x, y, z;
			s >> tmp >> v[X] >> v[Y] >> v[Z];
			v[W] = 1.0f;
			verts.push_back(v);
		}

		if (line[0] == 'f') {
			int face[3];
			s >> tmp >> face[0] >> face[1] >> face[2];
			Triangle t = Triangle(verts[face[0] - 1], verts[face[1] - 1], verts[face[2] - 1]);
			tris.push_back(t);
		}
	}

	return true;
}

std::vector<Triangle>& Mesh::getTris() {
	return tris;
}