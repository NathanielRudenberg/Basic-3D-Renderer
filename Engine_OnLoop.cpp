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

	Matrix rotateX, rotateZ, rotateY;
	rotateX.matrix[0][0] = 1;
	rotateX.matrix[1][1] = cosf(theta * -1.0f);
	rotateX.matrix[1][2] = sinf(theta * -1.0f);
	rotateX.matrix[2][1] = -sinf(theta * -1.0f);
	rotateX.matrix[2][2] = cosf(theta * -1.0f);
	rotateX.matrix[3][3] = 1;

	rotateZ.matrix[0][0] = cosf(theta);
	rotateZ.matrix[0][1] = sinf(theta);
	rotateZ.matrix[1][0] = -sinf(theta);
	rotateZ.matrix[1][1] = cosf(theta);
	rotateZ.matrix[2][2] = 1;
	rotateZ.matrix[3][3] = 1;

	rotateY.matrix[0][0] = cosf(theta * -2.0f);
	rotateY.matrix[0][2] = -sinf(theta * -2.0f);
	rotateY.matrix[1][1] = 1;
	rotateY.matrix[2][0] = sinf(theta * -2.0f);
	rotateY.matrix[2][2] = cosf(theta * -2.0f);;
	rotateY.matrix[3][3] = 1;

	std::vector<Triangle> trisToRaster;

	for (auto tri : cube.tris) {
		Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX, triRotatedZXY;

		for (int i = 0; i < 3; i++) {
			// Rotate
			MultiplyMatrixVector(tri.v[i], triRotatedZ.v[i], rotateZ);
			MultiplyMatrixVector(triRotatedZ.v[i], triRotatedZX.v[i], rotateX);
			MultiplyMatrixVector(triRotatedZX.v[i], triRotatedZXY.v[i], rotateY);
		}

		triTranslated = triRotatedZXY;
		for (int i = 0; i < 3; i++) {
			// Translate into view space
			triTranslated.v[i].z = triRotatedZXY.v[i].z + 3.0f;
		}

		// Get normals
		Point3d normal, line1, line2;
		line1.x = triTranslated.v[1].x - triTranslated.v[0].x;
		line1.y = triTranslated.v[1].y - triTranslated.v[0].y;
		line1.z = triTranslated.v[1].z - triTranslated.v[0].z;

		line2.x = triTranslated.v[2].x - triTranslated.v[0].x;
		line2.y = triTranslated.v[2].y - triTranslated.v[0].y;
		line2.z = triTranslated.v[2].z - triTranslated.v[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x /= l;
		normal.y /= l;
		normal.z /= l;

		if (normal.x * (triTranslated.v[0].x - virtualCam.x) +
			normal.y * (triTranslated.v[0].y - virtualCam.y) + 
			normal.z * (triTranslated.v[0].z - virtualCam.z) < 0.0f) {

			// Illumination
			Point3d lightDirection{ 0.0f, -1.0f, 0.0f };
			float mag = sqrtf(lightDirection.x * lightDirection.x + lightDirection.y * lightDirection.y + lightDirection.z * lightDirection.z);
			lightDirection.x /= mag;
			lightDirection.y /= mag;
			lightDirection.z /= mag;

			float dotProd = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;
			float luminance = (int)(127.5f * (1.0 + dotProd));
			triTranslated.shade = luminance;

			for (int i = 0; i < 3; i++) {
				// Project onto screen
				MultiplyMatrixVector(triTranslated.v[i], triProjected.v[i], projection);

				// Normalize and scale into view
				triProjected.v[i].x += 1.0f;
				triProjected.v[i].y += 1.0f;

				triProjected.v[i].x *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.v[i].y *= 0.5f * (float)SCREEN_HEIGHT;

				// Draw vertices
				// SDL_RenderDrawPoint(renderer, (float)triProjected.v[i].x, (float)triProjected.v[i].y);
			}

			triProjected.shade = triTranslated.shade;

			// Store triangles for sorting
			trisToRaster.push_back(triProjected);
		}
	}

	// Sort tris from back to front
	std::sort(trisToRaster.begin(), trisToRaster.end(), [](Triangle& t1, Triangle& t2) {
		float z1 = (t1.v[0].z + t1.v[1].z + t1.v[2].z) / 3.0f;
		float z2 = (t2.v[0].z + t2.v[1].z + t2.v[2].z) / 3.0f;

		return z1 > z2;
	});

	for (auto &triProjected : trisToRaster) {
		SDL_SetRenderDrawColor(renderer, triProjected.shade, triProjected.shade, triProjected.shade, 255);
		rasterize(triProjected);

		if (true) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[0].x, (float)triProjected.v[0].y, (float)triProjected.v[1].x, (float)triProjected.v[1].y);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[1].x, (float)triProjected.v[1].y, (float)triProjected.v[2].x, (float)triProjected.v[2].y);
			SDL_RenderDrawLine(renderer, (float)triProjected.v[2].x, (float)triProjected.v[2].y, (float)triProjected.v[0].x, (float)triProjected.v[0].y);
		}
	}
}