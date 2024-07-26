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
#include "Slope.h"
#include "Plane.h"

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
		float x, y, z, w;
	};

	struct TriangleNoEigen {
		Point3d v[3];

		TriangleNoEigen(Triangle& tri) {
			v[0] = { tri.getVerts()[0][X], tri.getVerts()[0][Y], tri.getVerts()[0][Z], tri.getVerts()[0][W], };
			v[1] = { tri.getVerts()[1][X], tri.getVerts()[1][Y], tri.getVerts()[0][Z], tri.getVerts()[1][W], };
			v[2] = { tri.getVerts()[2][X], tri.getVerts()[2][Y], tri.getVerts()[0][Z], tri.getVerts()[2][W], };
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
	float cameraRotSpeed;
	float cameraMoveSpeed;
	bool slowMode = false;
	bool fastMode = false;
	bool showTriEdges = false;
	bool drawTriangles = true;
	int xMousePos, yMousePos;

private:
	Camera camera;
	std::vector<Model> objects;

private:
	const float pi = 3.14159f;
	float elapsedTime = 0;
	double* depthBuffer = nullptr;
	const int DEPTH_BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
	RowVector3f getTriangleNormal(Triangle& triTransformed);
	RowVector3f getCameraRay(RowVector3f& v);
	std::string windowTitle;
	int getLuminance(RowVector3f& normal);
	void clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster);

public:
	template<typename V>
	void rasterizeTriangle(const V* v0, const V* v1, const V* v2, auto&& getXY, auto&& makeSlope, auto&& drawScanline);
	void rasterize(TriangleNoEigen& triangle);
	void render(Model& obj, Matrix4f viewMatrix, float translateX = 0.0f, float translateY = 0.0f, float translateZ = 0.0f);

public:
	Engine();
	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void CheckKeystate();
	void OnExit();
	void OnKeyDown(SDL_Keycode sym, Uint16 mod);
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	void OnMButtonDown(int mX, int mY);
	void OnMButtonUp(int mX, int mY);
	void OnLoop();
	void OnRender();
	void OnCleanup();
};