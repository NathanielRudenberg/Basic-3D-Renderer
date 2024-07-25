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

	std::vector<Triangle> trisToRaster;

	for (Triangle& tri : obj.getMesh().getTris()) {
		Triangle triTransformed, triViewed, triProjected;

		for (int i = 0; i < 3; i++) {
			// Transform
			triTransformed.getVerts()[i] = tri.getVerts()[i] * worldMatrix;
		}

		// Get normals
		RowVector3f normal, line1, line2, v1, v2, v3, cameraRay;
		v1 << triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z];
		v2 << triTransformed.getVerts()[1][X], triTransformed.getVerts()[1][Y], triTransformed.getVerts()[1][Z];
		v3 << triTransformed.getVerts()[2][X], triTransformed.getVerts()[2][Y], triTransformed.getVerts()[2][Z];

		cameraRay = v1 - camera.getPos();
		line1 = v2 - v1;
		line2 = v3 - v1;
		normal = line1.cross(line2).normalized();

		if (normal.dot(cameraRay) < 0.0f) {

			// Illumination
			RowVector3f lightDirection{ 0.5f, 1.0f, 0.0f };
			lightDirection = lightDirection.normalized();

			float dotProd = normal.dot(lightDirection);
			int luminance = (int)(127.5f * (1.0 + dotProd));

			for (int i = 0; i < 3; i++) {
				// Convert from world space to view space
				triViewed.getVerts()[i] = triTransformed.getVerts()[i] * viewMatrix;
			}

			triViewed.setLuminance(luminance);

			int clippedTriangleNum = 0;
			Triangle clipped[2];
			RowVector3f planePoint{ 0.0f, 0.0f, 0.1f };
			RowVector3f planeNormal{ 0.0f, 0.0f, 1.0f };
			clippedTriangleNum = clipTriangleAgainstPlane(planePoint, planeNormal, triViewed, clipped[0], clipped[1]);

			for (int n = 0; n < clippedTriangleNum; n++) {

				for (int i = 0; i < 3; i++) {
					// Project onto screen
					float nearPlane = 0.1f;
					float farPlane = 1000.0f;
					float fov = 80.0f;
					float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
					float aspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;

					triProjected.getVerts()[i] = project(clipped[n].getVerts()[i], fovRad, aspectRatio, nearPlane, farPlane);
					//triProjected.v[i][Z] = triProjected.v[i][W];

					// Normalize and scale into view
					triProjected.getVerts()[i][X] += 1.0f;
					triProjected.getVerts()[i][Y] += 1.0f;

					triProjected.getVerts()[i][X] *= 0.5f * (float)SCREEN_WIDTH;
					triProjected.getVerts()[i][Y] *= 0.5f * (float)SCREEN_HEIGHT;
				}

				triProjected.setLuminance(clipped[n].getLuminance());

				// Store triangles for sorting
				trisToRaster.push_back(triProjected);
			}
		}
	}

	// Sort tris from back to front
	/*std::sort(trisToRaster.begin(), trisToRaster.end(), [](Triangle& t1, Triangle& t2) {
		float w1 = (t1.getVerts()[0][W] + t1.getVerts()[1][W] + t1.getVerts()[2][W]) / 3.0f;
		float w2 = (t2.getVerts()[0][W] + t2.getVerts()[1][W] + t2.getVerts()[2][W]) / 3.0f;

		return w1 > w2;
		});*/

	for (auto& triToRaster : trisToRaster) {
		// Clip triangles against all screen edges
		Triangle clipped[2];
		std::list<Triangle> listTriangles;
		listTriangles.push_back(triToRaster);
		int newTrianglesNum = 1;

		for (int i = 0; i < 4; i++) {
			int numTrisToAdd = 0;
			while (newTrianglesNum > 0) {
				Triangle test = listTriangles.front();
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

		for (Triangle& t : listTriangles) {
			if (true) {
				SDL_SetRenderDrawColor(renderer, t.getLuminance(), t.getLuminance(), t.getLuminance(), 255);
				TriangleNoEigen toRaster = TriangleNoEigen(t);
				rasterize(toRaster);
			}

			// Draw triangles
			if (true) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawLine(renderer, (int)t.getVerts()[0][X], (int)t.getVerts()[0][Y], (int)t.getVerts()[1][X], (int)t.getVerts()[1][Y]);
				SDL_RenderDrawLine(renderer, (int)t.getVerts()[1][X], (int)t.getVerts()[1][Y], (int)t.getVerts()[2][X], (int)t.getVerts()[2][Y]);
				SDL_RenderDrawLine(renderer, (int)t.getVerts()[2][X], (int)t.getVerts()[2][Y], (int)t.getVerts()[0][X], (int)t.getVerts()[0][Y]);
			}

			if (false) {
				SDL_SetRenderDrawColor(renderer, t.getLuminance(), t.getLuminance(), 0, 255);
				SDL_RenderDrawPoint(renderer, (int)t.getVerts()[0][X], (int)t.getVerts()[0][Y]);
				SDL_RenderDrawPoint(renderer, (int)t.getVerts()[1][X], (int)t.getVerts()[1][Y]);
				SDL_RenderDrawPoint(renderer, (int)t.getVerts()[2][X], (int)t.getVerts()[2][Y]);
			}
		}
	}
}