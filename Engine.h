#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <Eigen/Core>
#include <iostream>

using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::RowVector2f;
using Eigen::RowVector3f;
using Eigen::RowVector4f;

class Engine {
private:
	enum coordIndices {
		X,
		Y,
		Z,
		W
	};

	struct Point3d {
		float x, y, z;
	};

	struct Trigon {
		RowVector4f v[3];
		int luminance;
	};

	struct TriangleNoEigen {
		Point3d v[3];

		TriangleNoEigen(Trigon& tri) {
			v[0] = { tri.v[0][X], tri.v[0][Y], tri.v[0][Z], };
			v[1] = { tri.v[1][X], tri.v[1][Y], tri.v[1][Z], };
			v[2] = { tri.v[2][X], tri.v[2][Y], tri.v[2][Z], };
		}
	};

	struct MatMesh {
		std::vector<Trigon> tris;

		bool loadObj(std::string filename) {
			std::ifstream f(filename);
			if (!f.is_open()) { return false; }

			// Build vertex cache
			std::vector<RowVector4f> verts;
			while (!f.eof()) {
				char line[128];
				f.getline(line, 128);

				std::strstream s;
				s << line;

				char tmp;
				if (line[0] == 'v') {
					RowVector4f v;
					s >> tmp >> v[X] >> v[Y] >> v[Z];
					v[W] = 1.0f;
					verts.push_back(v);
				}

				if (line[0] == 'f') {
					int f[3];
					s >> tmp >> f[0] >> f[1] >> f[2];
					tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
				}
			}

			return true;
		}
	};

	struct Matrix {
		float matrix[4][4] = { 0.0f };
	};

private:
	bool running;
	SDL_Surface* displaySurface = NULL;
	SDL_Surface* buffSurface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 700;
	float theta = 0.0f;

private:
	MatMesh matCube;
	MatMesh matExternal;
	Matrix4f projMat = Matrix4f::Zero();
	RowVector3f virtCam = RowVector3f::Zero();

private:
	bool naivePointInTriangle(TriangleNoEigen& tri, Point3d& point);
	bool doesTriangleContainPoint(TriangleNoEigen& tri, Point3d& point, float epsilon);
	float sqrPointDistanceToSegment(Point3d& pos1, Point3d& pos2, Point3d& point);

public:
	Engine();
	int OnExecute();
	void rasterize(TriangleNoEigen& triangle);

public:
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop(int elapsedTime);
	void OnRender();
	void OnCleanup();
};