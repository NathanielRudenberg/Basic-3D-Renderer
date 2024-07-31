#include "Renderer.h"

Renderer::Renderer() {};

Renderer::Renderer(Window* window, float cameraRotSpeed, float cameraMoveSpeed) :
	_window(window),
	_cameraRotSpeed(cameraRotSpeed),
	_cameraMoveSpeed(cameraMoveSpeed),
	_near(Plane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f })),
	_far(Plane({ 0.0f, 0.0f, 1000.0f }, { 0.0f, 0.0f, -1.0f })),
	_top(Plane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })),
	_bottom(Plane({ 0.0f, (float)(_window->height() - 1), 0.0f }, { 0.0f, -1.0f, 0.0f })),
	_left(Plane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f })),
	_right(Plane({ (float)(_window->width() - 1), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f })) {}

void Renderer::setCameraRotSpeed(float speed) {
	_cameraRotSpeed = speed;
}

void Renderer::setCameraMoveSpeed(float speed) {
	_cameraMoveSpeed = speed;
}

Camera& Renderer::camera() {
	return _camera;
}

float Renderer::getCameraRotSpeed() {
	if (slowRotateMode) {
		return 0.04f;
	}
	return 0.4f;
}

float Renderer::getCameraMoveSpeed() {
	if (fastMode) {
		return 8.0f;
	}
	else if (slowMode) {
		return 0.1f;
	}
	return 2.0f;
}

bool Renderer::getFastMode() const {
	return fastMode;
}

bool Renderer::getSlowMode() const {
	return slowMode;
}

bool Renderer::getSlowRotateMode() {
	return slowRotateMode;
}

void Renderer::setFastMode(bool mode) {
	fastMode = mode;
}

void Renderer::setSlowMode(bool mode) {
	slowMode = mode;
}

void Renderer::toggleSlowRotateMode() {
	slowRotateMode = !slowRotateMode;
}

void Renderer::toggleFastMode() {
	fastMode = !fastMode;
}

void Renderer::toggleSlowMode() {
	slowMode = !slowMode;
}

void Renderer::toggleTriEdges() {
	showTriEdges = !showTriEdges;
}

void Renderer::toggleDrawing() {
	drawTriangles = !drawTriangles;
}

vec3 Renderer::getTriangleNormal(Triangle& triTransformed) {
	vec3 normal, line1, line2, v1, v2, v3, cameraRay;
	v1 = vec3(triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z]);
	v2 = vec3(triTransformed.getVerts()[1][X], triTransformed.getVerts()[1][Y], triTransformed.getVerts()[1][Z]);
	v3 = vec3(triTransformed.getVerts()[2][X], triTransformed.getVerts()[2][Y], triTransformed.getVerts()[2][Z]);

	cameraRay = getCameraRay(v1);
	line1 = v2 - v1;
	line2 = v3 - v1;
	return normalize(cross(line1, line2));
}

vec3 Renderer::getCameraRay(const vec3& v) {
	return v - _camera.getPos();
}

int Renderer::getLuminance(const vec3& normal) {
	vec3 lightDirection{ 0.5f, 1.0f, 0.0f };
	lightDirection = normalize(lightDirection);

	float dotProd = dot(normal, lightDirection);
	int luminance = (int)(127.5f * (1.0 + dotProd));

	return luminance;
}

void Renderer::clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster) {
	Triangle clipped[2];
	int newTrianglesNum = 1;

	Plane* screenEdges[]{ &_top, &_bottom, &_left, &_right };
	for (Plane* edge : screenEdges) {
		int numTrisToAdd = 0;
		while (newTrianglesNum > 0) {
			Triangle test = trisToRaster.front();
			trisToRaster.pop_front();
			newTrianglesNum--;

			numTrisToAdd = clipTriangleAgainstPlane(*edge, test, clipped[0], clipped[1]);

			for (int w = 0; w < numTrisToAdd; w++) {
				trisToRaster.push_back(clipped[w]);
			}
		}
		newTrianglesNum = trisToRaster.size();
	}
}

void Renderer::render(Model& obj) {
	render(obj, obj.getPosition()[X], obj.getPosition()[Y], obj.getPosition()[Z]);
}

void Renderer::render(Model& obj, float translateX, float translateY, float translateZ) {
	vec3 targetVec = _camera.getPos() + _camera.getForward();
	mat4 cameraMatrix = getPointAtMatrix(_camera.getPos(), targetVec, _camera.getUp());
	mat4 viewMatrix = inverse(cameraMatrix);
	mat4 worldMatrix = getTranslationMatrix(translateX, translateY, translateZ);

	std::vector<Triangle> trisToClip;

	for (Triangle tri : obj.getMesh().getTris()) {
		Triangle& triTransformed = tri;

		// Translate the triangle into its position in world
		transformTriangle(triTransformed, worldMatrix);

		// Get triangle normals
		vec3 v = vec3(triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z]);
		vec3 normal = getTriangleNormal(triTransformed);

		// Only render triangles that are facing the camera
		if (dot(normal, getCameraRay(v)) < 0.0f) {

			// Illuminate the triangle
			triTransformed.setLuminance(getLuminance(normal));

			// Convert from world space to view space
			// i.e. translate the triangle into view
			transformTriangle(triTransformed, viewMatrix);

			int clippedTriangleNum = 0;
			Triangle clipped[2];
			clippedTriangleNum = clipTriangleAgainstPlane(_near, triTransformed, clipped[0], clipped[1]);

			for (int n = 0; n < clippedTriangleNum; n++) {
				projectTriangle(clipped[n], _window->width(), _window->height(), _near, _far);

				// Store triangles for clipping against screen edges
				trisToClip.push_back(clipped[n]);
			}
		}
	}

	// Sort tris from back to front
	/*std::sort(trisToRaster.begin(), trisToRaster.end(), [](Triangle& t1, Triangle& t2) {
		float w1 = (t1.getVerts()[0][W] + t1.getVerts()[1][W] + t1.getVerts()[2][W]) / 3.0f;
		float w2 = (t2.getVerts()[0][W] + t2.getVerts()[1][W] + t2.getVerts()[2][W]) / 3.0f;

		return w1 > w2;
		});*/

	for (auto& clippable : trisToClip) {
		std::list<Triangle> trisToRaster;
		trisToRaster.push_back(clippable);

		// Clip triangles against all screen edges
		clipAgainstScreenEdges(clippable, trisToRaster);

		for (Triangle& t : trisToRaster) {
			if (drawTriangles) {
				_window->setDrawColor(t.getLuminance(), t.getLuminance(), t.getLuminance(), 255);
				rasterize(t);
			}

			// Draw triangles
			if (showTriEdges) {
				_window->setDrawColor(255, 0, 0, 255);
				_window->drawLine((int)t.getVerts()[0][X], (int)t.getVerts()[0][Y], (int)t.getVerts()[1][X], (int)t.getVerts()[1][Y]);
				_window->drawLine((int)t.getVerts()[1][X], (int)t.getVerts()[1][Y], (int)t.getVerts()[2][X], (int)t.getVerts()[2][Y]);
				_window->drawLine((int)t.getVerts()[2][X], (int)t.getVerts()[2][Y], (int)t.getVerts()[0][X], (int)t.getVerts()[0][Y]);
			}

			if (false) {
				_window->setDrawColor(255, 255, 0, 255);
				_window->drawPoint((int)t.getVerts()[0][X], (int)t.getVerts()[0][Y]);
				_window->drawPoint((int)t.getVerts()[1][X], (int)t.getVerts()[1][Y]);
				_window->drawPoint((int)t.getVerts()[2][X], (int)t.getVerts()[2][Y]);
			}
		}
	}
}

template<typename V>
void Renderer::rasterizeTriangle(const V* v0, const V* v1, const V* v2,
	auto&& getXY,
	auto&& makeSlope,
	auto&& drawScanline)
	requires std::invocable<decltype(getXY), const V&>
		 and std::invocable<decltype(makeSlope), const V*, const V*, int>
		 and (std::tuple_size_v<std::remove_cvref_t<decltype(getXY(*v0))>> == 2)
			 and requires { { +std::get<0>(getXY(*v0)) } -> std::integral; }
			 and requires { { +std::get<1>(getXY(*v0)) } -> std::integral; }
			 and requires(std::remove_cvref_t<decltype(makeSlope(v0, v1, 1))> a) { drawScanline(1, a, a); }
		 {

	// Rasterize from top to bottom
	auto [x0, y0, x1, y1, x2, y2] = std::tuple_cat(getXY(*v0), getXY(*v1), getXY(*v2));

	// Sort the vertices by Y coordinate in descending order
	// Sort by X coordinate if the Y coordinates are equal
	if (std::tie(y1, x1) < std::tie(y0, x0)) { std::swap(x0, x1); std::swap(y0, y1); std::swap(v0, v1); }
	if (std::tie(y2, x2) < std::tie(y0, x0)) { std::swap(x0, x2); std::swap(y0, y2); std::swap(v0, v2); }
	if (std::tie(y2, x2) < std::tie(y1, x1)) { std::swap(x1, x2); std::swap(y1, y2); std::swap(v1, v2); }

	// Do nothing if the triangle has no area
	if (y0 == y2) return;

	// Find the short side.
	// If the bend is on the right, this evaluates to true.
	bool shortSide = (y1 - y0) * (x2 - x0) < (x1 - x0) * (y2 - y0);

	// Create two slopes: v0 to v1 (to the bend) and v0 to v2 (long side)
	std::invoke_result_t<decltype(makeSlope), V*, V*, int> sides[2] = {};
	sides[!shortSide] = makeSlope(v0, v2, y2 - y0);

	// Main rasterizing loop
	for (auto y = y0, endY = y0; ; ++y) {
		if (y >= endY) {
			if (y >= y2) break;
			sides[shortSide] = std::apply(makeSlope, (y < y1) ? std::tuple(v0, v1, (endY = y1) - y0)
															  : std::tuple(v1, v2, (endY = y2) - y1));
		}
		// Draw line of pixels
		drawScanline(y, sides[0], sides[1]);
	}
}

void Renderer::rasterize(Triangle& triangle) {
	vec4 v0 = triangle.getVerts()[0];
	vec4 v1 = triangle.getVerts()[1];
	vec4 v2 = triangle.getVerts()[2];

	using SlopeData = std::array<Slope, 2>; // x and depth-buffer

	rasterizeTriangle(&v0, &v1, &v2,
		// coord extractor
		[&](const auto& v) { return std::tuple{ (int)v[X], (int)v[Y]}; },
		// Slope generator
		[&](const auto* from, const auto* to, int numSteps) {
			SlopeData result;
			// Get begin and end X coordinates
			result[0] = Slope{ (float)(*from)[X], (float)(*to)[X], numSteps};
			// Get begin and end depth values
			result[1] = Slope{ (float)(*from)[Z], (float)(*to)[Z], numSteps};
			return result;
		},
		// Draw scanline
		[&](int y, SlopeData& left, SlopeData& right) {
			int x = (int)left[0].get(), endX = (int)right[0].get();

			// Horizontal interpolation for depth values
			Slope pixelDepth;
			int numSteps = endX - x;
			pixelDepth = Slope(left[1].get(), right[1].get(), numSteps);

			for (; x < endX; ++x) {
				// Update depth-buffer if the pixel is closer than the current buffer value
				int currentPixel = (y * _window->width()) + x;
				if (pixelDepth.get() < _window->getPixelDepth(currentPixel)) {
					_window->setPixelDepth(currentPixel, pixelDepth.get());
					_window->drawPoint(x, y);
				}
				pixelDepth.advance();
			}

			// Update the X and Z coordinates on both sides
			for (auto& slope : left) slope.advance();
			for (auto& slope : right) slope.advance();
		}
	);
}
