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
void Engine::rasterizeTriangle(const V* v0, const V* v1, const V* v2, auto&& getXY, auto&& makeSlope, auto&& drawScanline) {

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

	// Create two slops: v0 to v1 (to the bend) and v0 to v2 (long side)
	std::invoke_result_t<decltype(makeSlope), V*, V*, int> sides[2];
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

void Engine::rasterize(TriangleNoEigen& triangle) {
	// FillTriangle(triangle);
	std::array<int, 2> v0{ triangle.v[0].x, triangle.v[0].y };
	std::array<int, 2> v1{ triangle.v[1].x, triangle.v[1].y };
	std::array<int, 2> v2{ triangle.v[2].x, triangle.v[2].y };

	using SlopeData = std::pair<float /*begin*/, float /*step*/>;

	rasterizeTriangle(&v0, &v1, &v2,
		// coord extractor
		[&](const auto& v) { return v; },
		// Slope generator
		[&](const auto* from, const auto* to, int numSteps) {
			// Get X coordinates for begin and end
			int begin = (*from)[0], end = (*to)[0];
			// Number of steps is the number of scanlines
			float invSteps = 1.0f / numSteps;
			return SlopeData{ begin, (end - begin) * invSteps };
		},
		// Draw scanline
		[&](int y, SlopeData& left, SlopeData& right) {
			int startX = left.first, endX = right.first;
			for (int x = startX; x <= endX; ++x) {
				SDL_RenderDrawPoint(renderer, x, y);
			}

			// Update the X coordinate on both sides
			left.first += left.second;
			right.first += right.second;
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