#include "Engine.h"
#include <cmath>
#include <iostream>
#include <algorithm>

void Engine::OnLoop(int elapsedTime) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int center_x = SCREEN_WIDTH / 2;
	int center_y = SCREEN_HEIGHT / 2;
	int radius = 69;

	theta += 2.5f * (float)elapsedTime / 1000.0f / 3.0f;

	Eigen::Matrix4f rotX, rotY, rotZ, transform;
	rotX(0, 0) = 1;
	rotX(1, 1) = cosf(theta * -1.0f);
	rotX(1, 2) = sinf(theta * -1.0f);
	rotX(2, 1) = -sinf(theta * -1.0f);
	rotX(2, 2) = cosf(theta * -1.0f);
	rotX(3, 3) = 1;

	rotZ(0, 0) = cosf(theta);
	rotZ(0, 1) = sinf(theta);
	rotZ(1, 0) = -sinf(theta);
	rotZ(1, 1) = cosf(theta);
	rotZ(2, 2) = 1;
	rotZ(3, 3) = 1;

	rotY(0, 0) = cosf(theta * -2.0f);
	rotY(0, 2) = -sinf(theta * -2.0f);
	rotY(1, 1) = 1;
	rotY(2, 0) = sinf(theta * -2.0f);
	rotY(2, 2) = cosf(theta * -2.0f);;
	rotY(3, 3) = 1;

	transform = rotZ * rotY * rotX;

	std::vector<Trigon> trisToRaster;

	for (auto tri : matCube.tris) {
		Trigon triProjected, triTranslated, triRotatedZ, triRotatedZX, triRotatedZXY;

		for (int i = 0; i < 3; i++) {
			// Rotate
			triRotatedZXY.v[i] = tri.v[i] * transform;
		}

		triTranslated = triRotatedZXY;
		for (int i = 0; i < 3; i++) {
			// Translate into view space
			triTranslated.v[i][Z] = triRotatedZXY.v[i][Z] + 3.0f;
		}

		// Get normals
		Eigen::Vector3f normal, line1, line2;
		line1 << triTranslated.v[1];
		line2 << triTranslated.v[2];
		normal = line1.cross(line2).normalized();

		if (normal[X] * (triTranslated.v[0][X] - virtualCam.x) +
			normal[Y] * (triTranslated.v[0][Y] - virtualCam.y) +
			normal[Z] * (triTranslated.v[0][Z] - virtualCam.z) < 0.0f) {

			// Illumination
			Eigen::Vector3f lightDirection{ 0.0f, -1.0f, 0.0f };
			lightDirection.normalize();

			float dotProd = normal.dot(lightDirection);
			float luminance = (int)(127.5f * (1.0 + dotProd));
			triTranslated.luminance = luminance;

			for (int i = 0; i < 3; i++) {
				// Project onto screen
				triProjected.v[i] = triTranslated.v[i] * projMat;

				// Normalize and scale into view
				triProjected.v[i][X] += 1.0f;
				triProjected.v[i][Y] += 1.0f;

				triProjected.v[i][X] *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.v[i][Y] *= 0.5f * (float)SCREEN_HEIGHT;
			}

			triProjected.luminance = triTranslated.luminance;

			// Store triangles for sorting
			trisToRaster.push_back(triProjected);
		}
	}

	// Sort tris from back to front
	std::sort(trisToRaster.begin(), trisToRaster.end(), [](Trigon& t1, Trigon& t2) {
		float z1 = (t1.v[0][Z] + t1.v[1][Z] + t1.v[2][Z]) / 3.0f;
		float z2 = (t2.v[0][Z] + t2.v[1][Z] + t2.v[2][Z]) / 3.0f;

		return z1 > z2;
		});

	for (auto& triProjected : trisToRaster) {
		/*SDL_SetRenderDrawColor(renderer, triProjected.luminance, triProjected.luminance, triProjected.luminance, 255);
		rasterize(triProjected);*/

		if (true) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[0][X], (float)triProjected.v[0][Y], (float)triProjected.v[1][X], (float)triProjected.v[1][Y]);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[1][X], (float)triProjected.v[1][Y], (float)triProjected.v[2][X], (float)triProjected.v[2][Y]);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[2][X], (float)triProjected.v[2][Y], (float)triProjected.v[0][X], (float)triProjected.v[0][Y]);
		}
	}
}