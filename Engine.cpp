#include "Engine.h"
#include <algorithm>
#include <iterator>
// #include <algorithm>

Engine::Engine() {
	running = true;
}

auto t1 = SDL_GetTicks();
auto t2 = SDL_GetTicks();

void Engine::FillTriangle(TriangleNoEigen& tri) {
	int x1 = tri.v[0].x;
	int y1 = tri.v[0].y;
	float z1 = tri.v[0].z;
	int x2 = tri.v[1].x;
	int y2 = tri.v[1].y;
	float z2 = tri.v[1].z;
	int x3 = tri.v[2].x;
	int y3 = tri.v[2].y;
	float z3 = tri.v[2].z;
	auto swap = [](int& x, int& y) {int t = x; x = y; y = t; };
	auto swapf = [](float& x, float& y) {float t = x; x = y; y = t; };
	auto interpol = [](float x1, float y1, float x2, float x3, float y3) {
		return y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
	};
	auto drawLine = [&](int sx, int ex, float& sz, float& ez, int ny) {
		for (int i = sx; i < ex; i++) {
			float z = interpol(sx, sz, i, ex, ez);
			if (z < depthBuffer[ny * SCREEN_WIDTH + i]) {
				SDL_RenderDrawPoint(renderer, i, ny);
				depthBuffer[ny * SCREEN_WIDTH + i] = z;
			}
		}
	};

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	float startZ, endZ;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices in decreasing order of y value
	if (y1 > y2) { swap(y1, y2); swap(x1, x2); swapf(z1, z2); }
	if (y1 > y3) { swap(y1, y3); swap(x1, x3); swapf(z1, z3); }
	if (y2 > y3) { swap(y2, y3); swap(x2, x3); swapf(z2, z3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1);
	if (dx1 < 0) {
		dx1 = -dx1; signx1 = -1;
	}
	else {
		signx1 = 1;
	}
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1);
	if (dx2 < 0) {
		dx2 = -dx2; signx2 = -1;
	}
	else {
		signx2 = 1;
	}
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
		t1xp = 0;
		t2xp = 0;
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
		if (minx > t1x) {
			minx = t1x;
		}
		if (minx > t2x) {
			minx = t2x;
		}
		if (maxx < t1x) {
			maxx = t1x;
		}
		if (maxx < t2x) {
			maxx = t2x;
		}
		startZ = interpol((float)y1, z1, (float)y, (float)y2, z2);
		endZ = interpol((float)y1, z1, (float)y, (float)y3, z3);
		drawLine(minx, maxx, startZ, endZ, y);    // Draw line from min to max points found on the y
									// Now increase y
		if (!changed1) {
			t1x += signx1;
		}

		t1x += t1xp;
		if (!changed2) {
			t2x += signx2;
		}
		t2x += t2xp;
		y += 1;
		if (y == y2) {
			break;
		}
	}
next:
	// Second half
	// Triangle with flat top, pointy bottom
	dx1 = (int)(x3 - x2);
	if (dx1 < 0) {
		dx1 = -dx1; signx1 = -1;
	}
	else {
		signx1 = 1;
	}
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
			if (i < dx1) {
				i++;
			}
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

		if (minx > t1x) {
			minx = t1x;
		}
		if (minx > t2x) {
			minx = t2x;
		}
		if (maxx < t1x) {
			maxx = t1x;
		}
		if (maxx < t2x) {
			maxx = t2x;
		}


		startZ = interpol((float)y2, z2, (float)y, (float)y3, z3);
		endZ = interpol((float)y1, z1, (float)y, (float)y3, z3);
		drawLine(minx, maxx, startZ, endZ, y);
		if (!changed1) {
			t1x += signx1;
		}
		t1x += t1xp;
		if (!changed2) {
			t2x += signx2;
		}
		t2x += t2xp;
		y += 1;
		if (y > y3) {
			return;
		}
	}
}

void Engine::rasterize(TriangleNoEigen& triangle) {
	// FillTriangle(triangle);
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

Engine::~Engine() {
	delete depthBuffer;
	depthBuffer = nullptr;
}