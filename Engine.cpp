#include "Engine.h"
#include <algorithm>
#include <iterator>
// #include <algorithm>

Engine::Engine() {
	running = true;
}

auto t1 = SDL_GetTicks();
auto t2 = SDL_GetTicks();

template<typename V>
void Engine::rasterizeTriangle(const V* v0, const V* v1, const V* v2,
	auto&& getXY,
	auto&& makeSlope,
	auto&& drawScanline) {

	// Rasterize from top to bottom
	auto [x0,y0, x1,y1, x2,y2] = std::tuple_cat(getXY(*v0), getXY(*v1), getXY(*v2));

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
	std::invoke_result_t<decltype(makeSlope), V*, V*, int> sides[2];
	sides[!shortSide] = makeSlope(v0, v2, y2 - y0);

	// Main rasterizing loop
	for (auto y = y0, endY = y0; ; y++) {
		if (y >= endY) {
			if (y >= y2) break;
			sides[shortSide] = std::apply(makeSlope, (y < y1) ? std::tuple(v0, v1, (endY = y1) - y0)
															  : std::tuple(v1, v2, (endY = y2) - y1));
		}
		// Draw line of pixels
		drawScanline(y, sides[0], sides[1]);
	}
}

void Engine::rasterize(TriangleNoEigen& triangle) {
	// FillTriangle(triangle);
	Point3d v0 = triangle.v[0];
	Point3d v1 = triangle.v[1];
	Point3d v2 = triangle.v[2];
	/*std::array<int, 4> v0{ triangle.v[0].x, triangle.v[0].y, triangle.v[0].z, triangle.v[0].w };
	std::array<int, 4> v1{ triangle.v[1].x, triangle.v[1].y, triangle.v[1].z, triangle.v[1].w };
	std::array<int, 4> v2{ triangle.v[2].x, triangle.v[2].y, triangle.v[2].z, triangle.v[2].w };*/

	using SlopeData = std::array<Slope,2>; // x and depth-buffer

	rasterizeTriangle(&v0, &v1, &v2,
		// coord extractor
		[&](const auto& v) { return std::tuple{ v.x, v.y }; },
		// Slope generator
		[&](const auto* from, const auto* to, int numSteps) {
			SlopeData result;
			// Get begin and end X coordinates
			result[0] = Slope{ (float)(*from).x, (float)(*to).x, numSteps };
			// Get begin and end depth values
			result[1] = Slope{ (*from).w, (*to).w, numSteps };
			return result;
		},
		// Draw scanline
		[&](int y, SlopeData& left, SlopeData& right) {
			int x = left[0].get(), endX = right[0].get();

			// Horizontal interpolation for depth values
			Slope pixelDepth;
			int numSteps = endX - x;
			pixelDepth = Slope(left[1].get(), right[1].get(), numSteps);

			for (; x <= endX; x++) {
				// Update depth-buffer if the pixel is closer than the current buffer value
				int currentPixel = (y * SCREEN_WIDTH) + x;
				if (pixelDepth.get() < depthBuffer[currentPixel]) {
					depthBuffer[currentPixel] = pixelDepth.get();
					SDL_RenderDrawPoint(renderer, x, y);
				}
				pixelDepth.advance();
			}

			// Update the X and Z coordinates on both sides
			for (auto& slope : left) slope.advance();
			for (auto& slope : right) slope.advance();
		}
	);
}

int Engine::OnExecute() {
	if (!OnInit()) {
		return -1;
	}

	while (running) {
		t2 = SDL_GetTicks();
		elapsedTime = (t2 - t1) / 1000.0f;
		t1 = t2;

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			OnEvent(&event);
		}

		CheckKeystate();
		OnLoop();
		OnRender();
	}

	OnCleanup();

	return 0;
}

int main(int argc, char* argv[]) {
	Engine engine;

	return engine.OnExecute();
}