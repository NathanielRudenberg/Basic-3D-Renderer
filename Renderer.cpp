#include "Renderer.h"

//RowVector3f Renderer::getTriangleNormal(Triangle& triTransformed) {
//	RowVector3f normal, line1, line2, v1, v2, v3, cameraRay;
//	v1 << triTransformed.getVerts()[0][X], triTransformed.getVerts()[0][Y], triTransformed.getVerts()[0][Z];
//	v2 << triTransformed.getVerts()[1][X], triTransformed.getVerts()[1][Y], triTransformed.getVerts()[1][Z];
//	v3 << triTransformed.getVerts()[2][X], triTransformed.getVerts()[2][Y], triTransformed.getVerts()[2][Z];
//
//	cameraRay = getCameraRay(v1);
//	line1 = v2 - v1;
//	line2 = v3 - v1;
//	return line1.cross(line2).normalized();
//}
//
//RowVector3f Renderer::getCameraRay(RowVector3f& v) {
//	return v - camera.getPos();
//}
//
//int Renderer::getLuminance(RowVector3f& normal) {
//	RowVector3f lightDirection{ 0.5f, 1.0f, 0.0f };
//	lightDirection = lightDirection.normalized();
//
//	float dotProd = normal.dot(lightDirection);
//	int luminance = (int)(127.5f * (1.0 + dotProd));
//
//	return luminance;
//}
//
//void Renderer::clipAgainstScreenEdges(Triangle& clippable, std::list<Triangle>& trisToRaster) {
//	Triangle clipped[2];
//	int newTrianglesNum = 1;
//
//	Plane top(RowVector3f{ 0.0f, 0.0f, 0.0f }, RowVector3f{ 0.0f, 1.0f, 0.0f });
//	Plane bottom(RowVector3f{ 0.0f, (float)(SCREEN_HEIGHT - 1), 0.0f }, RowVector3f{ 0.0f, -1.0f, 0.0f });
//	Plane left(RowVector3f{ 0.0f, 0.0f, 0.0f }, RowVector3f{ 1.0f, 0.0f, 0.0f });
//	Plane right(RowVector3f{ (float)(SCREEN_WIDTH - 1), 0.0f, 0.0f }, RowVector3f{ -1.0f, 0.0f, 0.0f });
//
//	Plane screenEdges[]{ top, bottom, left, right };
//	for (Plane& edge : screenEdges) {
//		int numTrisToAdd = 0;
//		while (newTrianglesNum > 0) {
//			Triangle test = trisToRaster.front();
//			trisToRaster.pop_front();
//			newTrianglesNum--;
//
//			numTrisToAdd = clipTriangleAgainstPlane(edge.point(), edge.normal(), test, clipped[0], clipped[1]);
//
//			for (int w = 0; w < numTrisToAdd; w++) {
//				trisToRaster.push_back(clipped[w]);
//			}
//		}
//		newTrianglesNum = trisToRaster.size();
//	}
//}