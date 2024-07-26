#pragma once
#define PI 3.14159f
#include <Eigen/Core>
#include "Triangle.h"
#include "model.h"
#include "camera.h"
#include "Plane.h"
#include "Slope.h"
#include "TransformUtilities.h"
#include "Window.h"

class Renderer {
private:
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

	Camera _camera;
	Window* _window = NULL;
	float _cameraRotSpeed;
	float _cameraMoveSpeed;
	bool slowMode = false;
	bool fastMode = false;
	bool slowRotateMode = false;
	bool showTriEdges = false;
	bool drawTriangles = true;
	RowVector3f getTriangleNormal(Triangle& triTransformed);
	RowVector3f getCameraRay(RowVector3f& v);
	int getLuminance(RowVector3f& normal);
	void clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster);
	template<typename V>
	void rasterizeTriangle(const V* v0, const V* v1, const V* v2, auto&& getXY, auto&& makeSlope, auto&& drawScanline);
	void rasterize(TriangleNoEigen& triangle);

public:
	Renderer();
	Renderer(Window* window, float cameraRotSpeed, float cameraMoveSpeed);
	void setCameraRotSpeed(float speed);
	void setCameraMoveSpeed(float speed);
	Camera& camera();
	float getCameraRotSpeed();
	float getCameraMoveSpeed();
	bool getFastMode() const;
	bool getSlowMode() const;
	bool getSlowRotateMode();
	void setFastMode(bool mode);
	void setSlowMode(bool mode);
	void toggleSlowRotateMode();
	void toggleFastMode();
	void toggleSlowMode();
	void toggleTriEdges();
	void toggleDrawing();
	void render(Model& obj, Eigen::Matrix4f viewMatrix, float translateX = 0.0f, float translateY = 0.0f, float translateZ = 0.0f);
};

