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

RowVector3f Engine::getTriangleNormal(Triangle& triTransformed) {
	RowVector3f normal, line1, line2, v1, v2, v3, cameraRay;
	v1 << triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z];
	v2 << triTransformed.getVerts()[1][X], triTransformed.getVerts()[1][Y], triTransformed.getVerts()[1][Z];
	v3 << triTransformed.getVerts()[2][X], triTransformed.getVerts()[2][Y], triTransformed.getVerts()[2][Z];

	cameraRay = getCameraRay(v1);
	line1 = v2 - v1;
	line2 = v3 - v1;
	return line1.cross(line2).normalized();
}

RowVector3f Engine::getCameraRay(RowVector3f& v) {
	return v - camera.getPos();
}

int Engine::getLuminance(RowVector3f& normal) {
	RowVector3f lightDirection{ 0.5f, 1.0f, 0.0f };
	lightDirection = lightDirection.normalized();

	float dotProd = normal.dot(lightDirection);
	int luminance = (int)(127.5f * (1.0 + dotProd));

	return luminance;
}

void Engine::clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster) {
	Triangle clipped[2];
	int newTrianglesNum = 1;

	Plane top(RowVector3f{ 0.0f, 0.0f, 0.0f }, RowVector3f{ 0.0f, 1.0f, 0.0f });
	Plane bottom(RowVector3f{ 0.0f, (float)(SCREEN_HEIGHT - 1), 0.0f }, RowVector3f{ 0.0f, -1.0f, 0.0f });
	Plane left(RowVector3f{ 0.0f, 0.0f, 0.0f }, RowVector3f{ 1.0f, 0.0f, 0.0f });
	Plane right(RowVector3f{ (float)(SCREEN_WIDTH - 1), 0.0f, 0.0f }, RowVector3f{ -1.0f, 0.0f, 0.0f });

	Plane screenEdges[]{ top, bottom, left, right };
	for (Plane& edge : screenEdges) {
		int numTrisToAdd = 0;
		while (newTrianglesNum > 0) {
			Triangle test = trisToRaster.front();
			trisToRaster.pop_front();
			newTrianglesNum--;

			numTrisToAdd = clipTriangleAgainstPlane(edge.point(), edge.normal(), test, clipped[0], clipped[1]);

			for (int w = 0; w < numTrisToAdd; w++) {
				trisToRaster.push_back(clipped[w]);
			}
		}
		newTrianglesNum = trisToRaster.size();
	}
}

void Engine::render(Model& obj, Matrix4f viewMatrix, float translateX, float translateY, float translateZ) {
	Matrix4f translation = getTranslationMatrix(translateX, translateY, translateZ);
	Matrix4f worldMatrix = Matrix4f::Zero();

	worldMatrix = translation;

	std::vector<Triangle> trisToClip;

	for (Triangle& tri : obj.getMesh().getTris()) {
		Triangle triTransformed, triViewed, triProjected;

		for (int i = 0; i < 3; i++) {
			// Transform
			triTransformed.getVerts()[i] = tri.getVerts()[i] * worldMatrix;
		}

		// Get triangle normals
		RowVector3f v;
		v << triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z];
		RowVector3f normal = getTriangleNormal(triTransformed);

		// Only render triangles that are facing the camera
		if (normal.dot(getCameraRay(v)) < 0.0f) {

			// Illuminate the triangle
			triViewed.setLuminance(getLuminance(normal));

			// Convert from world space to view space
			for (int i = 0; i < 3; i++) {
				triViewed.getVerts()[i] = triTransformed.getVerts()[i] * viewMatrix;
			}

			int clippedTriangleNum = 0;
			Triangle clipped[2];
			Plane near = Plane(RowVector3f{ 0.0f, 0.0f, 0.1f }, RowVector3f{ 0.0f, 0.0f, 1.0f });
			Plane far = Plane(RowVector3f{ 0.0f, 0.0f, 1000.0f }, RowVector3f{ 0.0f, 0.0f, -1.0f });
			clippedTriangleNum = clipTriangleAgainstPlane(near.point(), near.normal(), triViewed, clipped[0], clipped[1]);

			float fov = 80.0f;
			float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
			float aspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;

			for (int n = 0; n < clippedTriangleNum; n++) {

				for (int i = 0; i < 3; i++) {
					// Project onto screen
					triProjected.getVerts()[i] = project(clipped[n].getVerts()[i], fovRad, aspectRatio, near.point()[Z], far.point()[Z]);
					//triProjected.getVerts()[i][Z] = triProjected.getVerts()[i][W];

					// Normalize and scale into view
					triProjected.getVerts()[i][X] += 1.0f;
					triProjected.getVerts()[i][Y] += 1.0f;

					triProjected.getVerts()[i][X] *= 0.5f * (float)SCREEN_WIDTH;
					triProjected.getVerts()[i][Y] *= 0.5f * (float)SCREEN_HEIGHT;
				}

				triProjected.setLuminance(clipped[n].getLuminance());

				// Store triangles for clipping against screen edges
				trisToClip.push_back(triProjected);
			}
		}
	}

	// Sort tris from back to front
	/*std::sort(trisToRaster.begin(), trisToRaster.end(), [](Triangle& t1, Triangle& t2) {
		float w1 = (t1.getVerts()[0][W] + t1.getVerts()[1][W] + t1.getVerts()[2][W]) / 3.0f;
		float w2 = (t2.getVerts()[0][W] + t2.getVerts()[1][W] + t2.getVerts()[2][W]) / 3.0f;

		return w1 > w2;
		});*/

	for (auto& clippable : trisToClip) {
		std::list<Triangle> trisToRaster;
		trisToRaster.push_back(clippable);

		// Clip triangles against all screen edges
		clipAgainstScreenEdges(clippable, trisToRaster);

		for (Triangle& t : trisToRaster) {
			if (drawTriangles) {
				SDL_SetRenderDrawColor(renderer, t.getLuminance(), t.getLuminance(), t.getLuminance(), 255);
				TriangleNoEigen toRaster = TriangleNoEigen(t);
				rasterize(toRaster);
			}

			// Draw triangles
			if (showTriEdges) {
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