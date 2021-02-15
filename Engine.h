#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <Eigen/Core>

class Engine {
private:
	enum coordIndices {
		X,
		Y,
		Z,
	};

	struct Point3d {
		float x, y, z;
	};

	struct Trigon {
		Eigen::RowVector3f v[3];
		int luminance;
	};

	struct Triangle {
		Point3d v[3];
		int shade;
	};

	struct MatMesh {
		std::vector<Trigon> tris;
	};

	struct Mesh {
		std::vector<Triangle> tris;

		bool loadObj(std::string filename) {
			std::ifstream f(filename);
			if (!f.is_open()) { return false; }

			// Build vertex cache
			std::vector<Point3d> verts;
			while (!f.eof()) {
				char line[128];
				f.getline(line, 128);

				std::strstream s;
				s << line;

				char tmp;
				if (line[0] == 'v') {
					Point3d v;
					s >> tmp >> v.x >> v.y >> v.z;
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
	Mesh cube;
	MatMesh matCube;
	Mesh external;
	Matrix projection;
	Eigen::Matrix4f projMat;
	Point3d virtualCam{0.0f, 0.0f, 0.0f};

private:
	void MultiplyMatrixVector(Point3d& i, Point3d& o, Matrix& m);
	bool naivePointInTriangle(Triangle tri, Point3d point);
	bool doesTriangleContainPoint(Triangle tri, Point3d point, float epsilon);
	float sqrPointDistanceToSegment(Point3d pos1, Point3d pos2, Point3d point);

public:
	Engine();
	int OnExecute();
	void rasterize(Triangle triangle);

public:
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop(int elapsedTime);
	void OnRender();
	void OnCleanup();
};