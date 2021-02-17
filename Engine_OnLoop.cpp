#include "Engine.h"
#include <cmath>
#include <algorithm>
#include <Eigen/Geometry>

void Engine::OnLoop(int elapsedTime) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int center_x = SCREEN_WIDTH / 2;
	int center_y = SCREEN_HEIGHT / 2;
	int radius = 69;

	theta += 2.5f * (float)elapsedTime / 1000.0f / 3.0f;

	Matrix4f rotX = Matrix4f::Zero(), rotY = Matrix4f::Zero(), rotZ = Matrix4f::Zero(), transform = Matrix4f::Zero();
	rotX(0, 0) = 1;
	rotX(1, 1) = cosf(theta * -2.0f);
	rotX(1, 2) = sinf(theta * -2.0f);
	rotX(2, 1) = -sinf(theta * -2.0f);
	rotX(2, 2) = cosf(theta * -2.0f);
	rotX(3, 3) = 1.0f;

	rotZ(0, 0) = cosf(theta);
	rotZ(0, 1) = sinf(theta);
	rotZ(1, 0) = -sinf(theta);
	rotZ(1, 1) = cosf(theta);
	rotZ(2, 2) = 1;
	rotZ(3, 3) = 1.0f;

	rotY(0, 0) = cosf(theta * -2.0f);
	rotY(0, 2) = -sinf(theta * -2.0f);
	rotY(1, 1) = 1;
	rotY(2, 0) = sinf(theta * -2.0f);
	rotY(2, 2) = cosf(theta * -2.0f);
	rotY(3, 3) = 1.0f;

	transform = rotX * rotY;
	transform = transform * rotZ;

	std::vector<Trigon> trisToRaster;

	for (auto& tri : matExternal.tris) {
		Trigon triProjected, triTranslated, triRotatedZ, triRotatedZX, triRotatedZXY;

		for (int i = 0; i < 3; i++) {
			// Rotate
			triRotatedZXY.v[i] = tri.v[i] * transform;
		}

		triTranslated = triRotatedZXY;
		for (int i = 0; i < 3; i++) {
			// Translate into view space
			triTranslated.v[i][Z] = triRotatedZXY.v[i][Z] + 6.0f;
		}

		// Get normals
		RowVector3f normal, line1, line2;
		line1[X] = triTranslated.v[1][X] - triTranslated.v[0][X];
		line1[Y] = triTranslated.v[1][Y] - triTranslated.v[0][Y];
		line1[Z] = triTranslated.v[1][Z] - triTranslated.v[0][Z];

		line2[X] = triTranslated.v[2][X] - triTranslated.v[0][X];
		line2[Y] = triTranslated.v[2][Y] - triTranslated.v[0][Y];
		line2[Z] = triTranslated.v[2][Z] - triTranslated.v[0][Z];
		normal = line1.cross(line2).normalized();

		if (normal[X] * (triTranslated.v[0][X] - virtCam[X]) +
			normal[Y] * (triTranslated.v[0][Y] - virtCam[Y]) +
			normal[Z] * (triTranslated.v[0][Z] - virtCam[Z]) < 0.0f) {

			// Illumination
			RowVector3f lightDirection{ 0.0f, 0.0f, -1.0f };
			lightDirection = lightDirection.normalized();

			float dotProd = normal.dot(lightDirection);
			int luminance = (int)(127.5f * (1.0 + dotProd));
			triTranslated.luminance = luminance;

			for (int i = 0; i < 3; i++) {
				// Project onto screen
				Eigen::RowVector4f tmp;
				tmp << triTranslated.v[i];
				Eigen::RowVector4f tmpProj = tmp * projMat;
				if (tmpProj[W] != 0.0f) {
					triProjected.v[i] << tmpProj[X] / tmpProj[W], tmpProj[Y] / tmpProj[W], tmpProj[Z] / tmpProj[W], 1.0f;
				}
				else {
					triProjected.v[i] << tmpProj[X], tmpProj[Y], tmpProj[Z], 1.0f;
				}

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
		if (true) {
			SDL_SetRenderDrawColor(renderer, triProjected.luminance, triProjected.luminance, triProjected.luminance, 255);
			TriangleNoEigen toRaster = TriangleNoEigen(triProjected);
			rasterize(toRaster);
		}

		if (false) {
			SDL_SetRenderDrawColor(renderer, triProjected.luminance, 0, 0, 255);
			SDL_RenderDrawLine(renderer, (int)triProjected.v[0][X], (int)triProjected.v[0][Y], (int)triProjected.v[1][X], (int)triProjected.v[1][Y]);
			SDL_RenderDrawLine(renderer, (int)triProjected.v[1][X], (int)triProjected.v[1][Y], (int)triProjected.v[2][X], (int)triProjected.v[2][Y]);
			SDL_RenderDrawLine(renderer, (int)triProjected.v[2][X], (int)triProjected.v[2][Y], (int)triProjected.v[0][X], (int)triProjected.v[0][Y]);
		}

		if (false) {
			SDL_SetRenderDrawColor(renderer, triProjected.luminance, triProjected.luminance, 0, 255);
			SDL_RenderDrawPoint(renderer, (int)triProjected.v[0][X], (int)triProjected.v[0][Y]);
			SDL_RenderDrawPoint(renderer, (int)triProjected.v[1][X], (int)triProjected.v[1][Y]);
			SDL_RenderDrawPoint(renderer, (int)triProjected.v[2][X], (int)triProjected.v[2][Y]);
		}
	}
}