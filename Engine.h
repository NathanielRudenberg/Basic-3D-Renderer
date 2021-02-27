#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <Eigen/Core>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include "EngineEvent.h"
#include "model.h"
#include "camera.h"

using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::RowVector2f;
using Eigen::RowVector3f;
using Eigen::RowVector4f;

class Engine : public EngineEvent {
public:
	enum coordIndices {
		X,
		Y,
		Z,
		W
	};

	struct Point3d {
		float x, y, z;
	};

	struct TriangleNoEigen {
		Point3d v[3];

		TriangleNoEigen(Triangle& tri) {
			v[0] = { tri.getVerts()[0][X], tri.getVerts()[0][Y], tri.getVerts()[0][Z], };
			v[1] = { tri.getVerts()[1][X], tri.getVerts()[1][Y], tri.getVerts()[1][Z], };
			v[2] = { tri.getVerts()[2][X], tri.getVerts()[2][Y], tri.getVerts()[2][Z], };
		}
	};

private:

	struct Matrix {
		float matrix[4][4] = { 0.0f };
	};
	bool running;
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 700;

private:
	Camera camera;
	std::vector<Model> objects;

private:
	const float pi = 3.14159f;
	float elapsedTime = 0;
	float* depthBuffer = nullptr;

public:
	void TriFill(TriangleNoEigen& tri);
	void FillTriangle(TriangleNoEigen& tri);
	void rasterize(TriangleNoEigen& triangle);
	void render(Model& obj, Matrix4f viewMatrix, float translateX = 0.0f, float translateY = 0.0f, float translateZ = 0.0f);

public:
	Engine();
	~Engine();
	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void CheckKeystate();
	void OnExit();
	void OnKeyDown(SDL_Keycode sym, Uint16 mod);
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	void OnLoop();
	void OnRender();
	void OnCleanup();
};