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

void Engine::FillTriangle(TriangleNoEigen& tri) {
	int x1 = tri.v[0].x;
	int y1 = tri.v[0].y;
	int x2 = tri.v[1].x;
	int y2 = tri.v[1].y;
	int x3 = tri.v[2].x;
	int y3 = tri.v[2].y;
	auto swap = [](int& x, int& y) {int t = x; x = y; y = t; };
	auto drawLine = [&](int startX, int endX, int ny) {
		//SDL_RenderDrawLine(renderer, startX, ny, endX, ny);

		// Interpolate Z values
 
		// Draw line of pixels
		for (int i = startX; i <= endX; i++) {
			SDL_RenderDrawPoint(renderer, i, ny);
		}
	};

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { swap(y1, y2); swap(x1, x2); }
	if (y1 > y3) { swap(y1, y3); swap(x1, x3); }
	if (y2 > y3) { swap(y2, y3); swap(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1);
	if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1);
	if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		swap(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		swap(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) {
		goto next;
	}
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) {
			minx = t1x; maxx = t2x;
		}
		else {
			minx = t2x; maxx = t1x;
		}
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) {
					t1xp = signx1;//t1x += signx1;
				}
				else {
					goto next1;
				}
			}
			if (changed1) {
				break;
			}
			else {
				t1x += signx1;
			}
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) {
					t2xp = signx2;//t2x += signx2;
				}
				else {
					goto next2;
				}
			}
			if (changed2) {
				break;
			}
			else {
				t2x += signx2;
			}
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(minx, maxx, y);    // Draw line from min to max points found on the y
									// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	// Triangle with flat top, pointy bottom
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		swap(dy1, dx1);
		changed1 = true;
	}
	else {
		changed1 = false;
	}

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) {
			minx = t1x; maxx = t2x;
		}
		else {
			minx = t2x; maxx = t1x;
		}
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) {
					t1xp = signx1; break; //t1x += signx1;
				}
				else {
					goto next3;
				}
			}
			if (changed1) {
				break;
			}
			else {
				t1x += signx1;
			}
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) {
					t2xp = signx2;
				}
				else {
					goto next4;
				}
			}
			if (changed2) {
				break;
			}
			else {
				t2x += signx2;
			}
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
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