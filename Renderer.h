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
	void rasterizeTriangle(const V* v0, const V* v1, const V* v2, auto&& getXY, auto&& makeSlope, auto&& drawScanline)
		requires std::invocable<decltype(getXY), const V&>
			 and std::invocable<decltype(makeSlope), const V*, const V*, int>
			 and (std::tuple_size_v<std::remove_cvref_t<decltype(getXY(*v0))>> == 2)
			 and requires { { +std::get<0>(getXY(*v0)) } -> std::integral; }
			 and requires { { +std::get<1>(getXY(*v0)) } -> std::integral; }
			 and requires(std::remove_cvref_t<decltype(makeSlope(v0, v1, 1))> a) { drawScanline(1, a, a); };
	void rasterize(Triangle& triangle);

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
	void render(Model& obj, Matrix4f viewMatrix);
	void render(Model& obj, Matrix4f viewMatrix, float translateX, float translateY, float translateZ);
};

