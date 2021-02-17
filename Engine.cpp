#include "Engine.h"
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
	/*if (sqrPointDistanceToSegment(v1, v2, point) <= epsilonSq) { return true; }
	if (sqrPointDistanceToSegment(v2, v3, point) <= epsilonSq) { return true; }
	if (sqrPointDistanceToSegment(v3, v1, point) <= epsilonSq) { return true; }*/

	return false;
}

void Engine::rasterize(TriangleNoEigen& triangle) {
	float epsilon = 0.45f;
	// Get triangle's bounding box
	float bbMinX = std::min(triangle.v[0].x, std::min(triangle.v[1].x, triangle.v[2].x)) - epsilon;
	float bbMinY = std::min(triangle.v[0].y, std::min(triangle.v[1].y, triangle.v[2].y)) + epsilon;
	float bbMaxX = std::max(triangle.v[0].x, std::max(triangle.v[1].x, triangle.v[2].x)) - epsilon;
	float bbMaxY = std::max(triangle.v[0].y, std::max(triangle.v[1].y, triangle.v[2].y)) + epsilon;

	for (float x = bbMinX; x <= bbMaxX; x++) {
		for (float y = bbMinY; y <= bbMaxY; y++) {
			Point3d pointToTest{ x, y, 0.0f };
			if (doesTriangleContainPoint(triangle, pointToTest, epsilon)) {
				SDL_RenderDrawPoint(renderer, (int)x, (int)y);
			}
		}
	}
}

int Engine::OnExecute() {
	if (!OnInit()) {
		return -1;
	}


	while (running) {
		t2 = SDL_GetTicks();
		float elapsedTime = (t2 - t1) / 1000.0f;
		t1 = t2;

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			OnEvent(&event, elapsedTime);
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