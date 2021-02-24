#define PI 3.14159f
#include "Engine.h"
#include <cmath>
#include <algorithm>
#include <list>
#include <Eigen/Geometry>
#include "TransformUtilities.h"

void Engine::OnRender() {
	SDL_RenderPresent(renderer);
}

void Engine::render(Model& obj, Matrix4f viewMatrix, float translateX, float translateY, float translateZ) {
	Matrix4f translation = getTranslationMatrix(translateX, translateY, translateZ);
	Matrix4f worldMatrix = Matrix4f::Zero();

	worldMatrix = translation;

	std::vector<Trigon> trisToRaster;

	for (Triangle& tri : obj.getMesh().getTris()) {
		Trigon triViewed, triTransformed, triProjected;

		for (int i = 0; i < 3; i++) {
			// Transform
			triTransformed.v[i] = tri.getVerts().at(i) * worldMatrix;
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

		if (normal[X] * (triTransformed.v[0][X] - camera.getPos()[X]) +
			normal[Y] * (triTransformed.v[0][Y] - camera.getPos()[Y]) +
			normal[Z] * (triTransformed.v[0][Z] - camera.getPos()[Z]) < 0.0f) {

			// Illumination
			RowVector3f lightDirection{ 0.5f, 1.0f, 0.0f };
			lightDirection = lightDirection.normalized();

			float dotProd = normal.dot(lightDirection);
			int luminance = (int)(127.5f * (1.0 + dotProd));

			for (int i = 0; i < 3; i++) {
				// Convert from world space to view space
				triViewed.v[i] = triTransformed.v[i] * viewMatrix;
			}

			triViewed.luminance = luminance;

			int clippedTriangleNum = 0;
			Trigon clipped[2];
			RowVector3f planePoint{ 0.0f, 0.0f, 0.1f };
			RowVector3f planeNormal{ 0.0f, 0.0f, 1.0f };
			clippedTriangleNum = clipTriangleAgainstPlane(planePoint, planeNormal, triViewed, clipped[0], clipped[1]);

			for (int n = 0; n < clippedTriangleNum; n++) {

				for (int i = 0; i < 3; i++) {
					// Project onto screen
					float nearPlane = 0.1f;
					float farPlane = 1000.0f;
					float fov = 75.0f;
					float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
					float aspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;

					triProjected.v[i] = project(clipped[n].v[i], fovRad, aspectRatio, nearPlane, farPlane);
					triProjected.v[i][Z] = triProjected.v[i][W];

					// Normalize and scale into view
					triProjected.v[i][X] += 1.0f;
					triProjected.v[i][Y] += 1.0f;

					triProjected.v[i][X] *= 0.5f * (float)SCREEN_WIDTH;
					triProjected.v[i][Y] *= 0.5f * (float)SCREEN_HEIGHT;
				}

				triProjected.luminance = clipped[n].luminance;

				// Store triangles for sorting
				trisToRaster.push_back(triProjected);
			}
		}
	}

	// Sort tris from back to front
	std::sort(trisToRaster.begin(), trisToRaster.end(), [](Trigon& t1, Trigon& t2) {
		float z1 = (t1.v[0][Z] + t1.v[1][Z] + t1.v[2][Z]) / 3.0f;
		float z2 = (t2.v[0][Z] + t2.v[1][Z] + t2.v[2][Z]) / 3.0f;

		return z1 > z2;
		});

	for (auto& triToRaster : trisToRaster) {
		// Clip triangles against all screen edges
		Trigon clipped[2];
		std::list<Trigon> listTriangles;
		listTriangles.push_back(triToRaster);
		int newTrianglesNum = 1;

		for (int i = 0; i < 4; i++) {
			int numTrisToAdd = 0;
			while (newTrianglesNum > 0) {
				Trigon test = listTriangles.front();
				listTriangles.pop_front();
				newTrianglesNum--;

				RowVector3f edge, edgeNorm;

				switch (i) {
				case 0:
					edge << 0.0f, 0.0f, 0.0f;
					edgeNorm << 0.0f, 1.0f, 0.0f;
					break;
				case 1:
					edge << 0.0f, (float)(SCREEN_HEIGHT - 1), 0.0f;
					edgeNorm << 0.0f, -1.0f, 0.0f;
					break;
				case 2:
					edge << 0.0f, 0.0f, 0.0f;
					edgeNorm << 1.0f, 0.0f, 0.0f;
					break;
				case 3:
					edge << (float)(SCREEN_WIDTH - 1), 0.0f, 0.0f;
					edgeNorm << -1.0f, 0.0f, 0.0f;
					break;
				}

				numTrisToAdd = clipTriangleAgainstPlane(edge, edgeNorm, test, clipped[0], clipped[1]);

				for (int w = 0; w < numTrisToAdd; w++) {
					listTriangles.push_back(clipped[w]);
				}
			}
			newTrianglesNum = listTriangles.size();
		}

		for (Trigon& t : listTriangles) {
			if (true) {
				SDL_SetRenderDrawColor(renderer, t.luminance, t.luminance, t.luminance, 255);
				TriangleNoEigen toRaster = TriangleNoEigen(t);
				rasterize(toRaster);
			}

			if (true) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawLine(renderer, (int)t.v[0][X], (int)t.v[0][Y], (int)t.v[1][X], (int)t.v[1][Y]);
				SDL_RenderDrawLine(renderer, (int)t.v[1][X], (int)t.v[1][Y], (int)t.v[2][X], (int)t.v[2][Y]);
				SDL_RenderDrawLine(renderer, (int)t.v[2][X], (int)t.v[2][Y], (int)t.v[0][X], (int)t.v[0][Y]);
			}

			if (false) {
				SDL_SetRenderDrawColor(renderer, t.luminance, t.luminance, 0, 255);
				SDL_RenderDrawPoint(renderer, (int)t.v[0][X], (int)t.v[0][Y]);
				SDL_RenderDrawPoint(renderer, (int)t.v[1][X], (int)t.v[1][Y]);
				SDL_RenderDrawPoint(renderer, (int)t.v[2][X], (int)t.v[2][Y]);
			}
		}
	}
}