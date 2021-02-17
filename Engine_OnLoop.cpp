#define PI 3.14159f

#include "Engine.h"
#include <cmath>
#include <algorithm>
#include <Eigen/Geometry>
#include "TransformUtilities.h"

void Engine::OnLoop(float elapsedTime) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int center_x = SCREEN_WIDTH / 2;
	int center_y = SCREEN_HEIGHT / 2;
	int radius = 69;

	//theta += 2.5f * (float)elapsedTime / 1000.0f;

	Matrix4f rotX = getXRot(theta);
	Matrix4f rotY = getYRot(theta);
	Matrix4f rotZ = getZRot(theta);
	Matrix4f translation = getTranslationMatrix(0.0f, 0.0f, 6.0f);
	Matrix4f worldMatrix = Matrix4f::Zero();

	worldMatrix = rotY * rotX;
	worldMatrix *= rotZ;
	worldMatrix *= translation;

	lookDir = { 0.0f, 0.0f, 1.0f };
	RowVector4f tmpLook, tmpCamLoc;
	tmpCamLoc << virtCam, 1.0f;
	RowVector3f upVec{ 0.0f, 1.0f, 0.0f };
	RowVector4f tmpTarg = { 0.0f, 0.0f, 1.0f, 1.0f };
	Matrix4f cameraRotation = getYRot(yaw);
	tmpLook = tmpTarg * cameraRotation;
	tmpTarg = tmpCamLoc + tmpLook;
	
	RowVector3f targetVec = virtCam + lookDir;

	lookDir[X] = tmpLook[X]; lookDir[Y] = tmpLook[Y]; lookDir[Z] = tmpLook[Z];
	targetVec[X] = tmpTarg[X]; targetVec[Y] = tmpTarg[Y]; targetVec[Z] = tmpTarg[Z];

	Matrix4f cameraMatrix = getPointAtMatrix(virtCam, targetVec, upVec);
	Matrix4f viewMatrix = cameraMatrix.inverse();

	std::vector<Trigon> trisToRaster;

	for (auto& tri : matExternal.tris) {
		Trigon triProjected, triTransformed, triViewed;

		for (int i = 0; i < 3; i++) {
			// Rotate
			triTransformed.v[i] = tri.v[i] * worldMatrix;
		}

		// Get normals
		RowVector3f normal, line1, line2;
		line1[X] = triTransformed.v[1][X] - triTransformed.v[0][X];
		line1[Y] = triTransformed.v[1][Y] - triTransformed.v[0][Y];
		line1[Z] = triTransformed.v[1][Z] - triTransformed.v[0][Z];

		line2[X] = triTransformed.v[2][X] - triTransformed.v[0][X];
		line2[Y] = triTransformed.v[2][Y] - triTransformed.v[0][Y];
		line2[Z] = triTransformed.v[2][Z] - triTransformed.v[0][Z];
		normal = line1.cross(line2).normalized();

		if (normal[X] * (triTransformed.v[0][X] - virtCam[X]) +
			normal[Y] * (triTransformed.v[0][Y] - virtCam[Y]) +
			normal[Z] * (triTransformed.v[0][Z] - virtCam[Z]) < 0.0f) {

			// Illumination
			RowVector3f lightDirection{ 0.0f, 0.0f, -1.0f };
			lightDirection = lightDirection.normalized();

			float dotProd = normal.dot(lightDirection);
			int luminance = (int)(127.5f * (1.0 + dotProd));
			triTransformed.luminance = luminance;

			for (int i = 0; i < 3; i++) {
				// Convert from world space to view space
				triViewed.v[i] = triTransformed.v[i] * viewMatrix;

				// Project onto screen
				float nearPlane = 0.1f;
				float farPlane = 1000.0f;
				float fov = 90.0f;
				float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
				float aspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;

				triProjected.v[i] = project(triViewed.v[i], fovRad, aspectRatio, nearPlane, farPlane);

				// Normalize and scale into view
				triProjected.v[i][X] += 1.0f;
				triProjected.v[i][Y] += 1.0f;

				triProjected.v[i][X] *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.v[i][Y] *= 0.5f * (float)SCREEN_HEIGHT;
			}

			triProjected.luminance = triTransformed.luminance;

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