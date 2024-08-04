#pragma once
#include <glm.hpp>
#include <list>
#include <array>
#include <algorithm>
#include "Triangle.h"
#include "model.h"
#include "camera.h"
#include "Frustum.h"
#include "Plane.h"
#include "Slope.h"
#include "TransformUtilities.h"
#include "Window.h"

using glm::vec4;

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
	bool slowMode = false;
	bool fastMode = false;
	bool slowRotateMode = false;
	bool showTriEdges = false;
	bool drawTriangles = true;
	Plane _near;
	Plane _far;
	Plane _top;
	Plane _bottom;
	Plane _left;
	Plane _right;
	Frustum _frustum;
	vec3 getTriangleNormal(Triangle& triTransformed);
	vec3 getCameraRay(const vec3& v);
	int getLuminance(const vec3& normal);
	void clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster);
	void clipAgainstFrustum(Triangle& clippable, Frustum& frustum, std::list<Triangle>& trisToProject);
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
	Renderer(Window* window);
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
	void render(std::vector<Model>& objects);
};

