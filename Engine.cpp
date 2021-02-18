#include "Engine.h"
#include <algorithm>
#include <iterator>
// #include <algorithm>

Engine::Engine() {
	running = true;
}

auto t1 = SDL_GetTicks();
auto t2 = SDL_GetTicks();

bool Engine::naivePointInTriangle(TriangleNoEigen& tri, Point3d& point) {
	float denominator = ((tri.v[1].y - tri.v[2].y) * (tri.v[0].x - tri.v[2].x) + (tri.v[2].x - tri.v[1].x) * (tri.v[0].y - tri.v[2].y));
	float a = ((tri.v[1].y - tri.v[2].y) * (point.x - tri.v[2].x) + (tri.v[2].x - tri.v[1].x) * (point.y - tri.v[2].y)) / denominator;
	float b = ((tri.v[2].y - tri.v[0].y) * (point.x - tri.v[2].x) + (tri.v[0].x - tri.v[2].x) * (point.y - tri.v[2].y)) / denominator;
	float c = 1 - a - b;

	return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}

float Engine::sqrPointDistanceToSegment(Point3d& pos1, Point3d& pos2, Point3d& point) {
	float sideLengthSquared = (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y);
	float dotProduct = ((point.x - pos1.x) * (pos2.x - pos1.x) + (point.y - pos1.y) * (pos2.y - pos1.y)) / sideLengthSquared;

	if (dotProduct < 0) {
		return (point.x - pos1.x) * (point.x - pos1.x) + (point.y - pos1.y) * (point.y - pos1.y);
	}
	else if (dotProduct <= 1) {
		float p_p1_squareLength = (pos1.x - point.x) * (pos1.x - point.x) + (pos1.y - point.y) * (pos1.y - point.y);
		return p_p1_squareLength - dotProduct * dotProduct * sideLengthSquared;
	}
	else {
		return (point.x - pos2.x) * (point.x - pos2.x) + (point.y - pos2.y) * (point.y - pos2.y);
	}
}

bool Engine::doesTriangleContainPoint(TriangleNoEigen& tri, Point3d& point, float epsilon) {
	Point3d& v1 = tri.v[0];
	Point3d& v2 = tri.v[1];
	Point3d& v3 = tri.v[2];

	//std::cout << "Checking point " << point << "\n";

	float epsilonSq = epsilon * epsilon;
	if (naivePointInTriangle(tri, point)) { return true; }
	if (sqrPointDistanceToSegment(v1, v2, point) <= epsilonSq) { return true; }
	if (sqrPointDistanceToSegment(v2, v3, point) <= epsilonSq) { return true; }
	if (sqrPointDistanceToSegment(v3, v1, point) <= epsilonSq) { return true; }

	return false;
}

void Engine::FillTriangle(TriangleNoEigen& tri) {
	int x1 = tri.v[0].x;
	int x2 = tri.v[1].x;
	int x3 = tri.v[2].x;
	int y1 = tri.v[0].y;
	int y2 = tri.v[1].y;
	int y3 = tri.v[2].y;
	auto swap = [](int& x, int& y) {int t = x; x = y; y = t; };
	auto drawLine = [&](int sx, int ex, int ny) { for (int i = sx; i < ex; i++) SDL_RenderDrawPoint(renderer, i, ny); };

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
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
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
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
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
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		swap(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
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
	FillTriangle(triangle);
	//DrawTriangle(triangle);
	//float epsilon = 0.45f;
	//// Get triangle's bounding box
	//float bbMinX = std::min(triangle.v[0].x, std::min(triangle.v[1].x, triangle.v[2].x)) - epsilon;
	//float bbMinY = std::min(triangle.v[0].y, std::min(triangle.v[1].y, triangle.v[2].y)) + epsilon;
	//float bbMaxX = std::max(triangle.v[0].x, std::max(triangle.v[1].x, triangle.v[2].x)) - epsilon;
	//float bbMaxY = std::max(triangle.v[0].y, std::max(triangle.v[1].y, triangle.v[2].y)) + epsilon;

	//for (float x = bbMinX; x <= bbMaxX; x++) {
	//	for (float y = bbMinY; y <= bbMaxY; y++) {
	//		Point3d pointToTest{ x, y, 0.0f };
	//		if (doesTriangleContainPoint(triangle, pointToTest, epsilon)) {
	//			SDL_RenderDrawPoint(renderer, (int)x, (int)y);
	//			//depthBuffer[(int)x * SCREEN_WIDTH + (int)y];
	//		}
	//	}
	//}
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
			OnEvent(&event, elapsedTime);
		}

		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		RowVector3f vForward = lookDir * (1.0f * elapsedTime);
		if (keystate[SDL_SCANCODE_SPACE]) {
			virtCam[Y] = virtCam[Y] + 1.0f * elapsedTime;
		}
		if (keystate[SDL_SCANCODE_LSHIFT]) {
			virtCam[Y] = virtCam[Y] - 1.0f * elapsedTime;
		}
		if (keystate[SDL_SCANCODE_A]) {
			virtCam[X] = virtCam[X] + 1.0f * elapsedTime;
		}
		if (keystate[SDL_SCANCODE_D]) {
			virtCam[X] = virtCam[X] - 1.0f * elapsedTime;
		}
		if (keystate[SDL_SCANCODE_W]) {
			virtCam = virtCam + vForward;
		}
		if (keystate[SDL_SCANCODE_S]) {
			virtCam = virtCam - vForward;
		}

		OnLoop(elapsedTime);
		OnRender();
	}

	OnCleanup();

	return 0;
}

int main(int argc, char* argv[]) {
	Engine engine;

	return engine.OnExecute();
}