#define PI 3.14159f

#include <Eigen/Geometry>
#include "Engine.h"
#include "TransformUtilities.h"
using std::string;
using std::to_string;

void Engine::OnLoop() {
	window.clear();

	float fps = 1.0f / elapsedTime;
	string fpsString = "FPS: " + to_string(fps);
	string mX = "X: " + to_string(xMousePos);
	string mY = "Y: " + to_string(yMousePos);
	string depth = "Depth: " + to_string( window.getDepthBuffer()[(yMousePos * SCREEN_WIDTH) + xMousePos]);
	string windowTitle = "Basic 3D Engine " + mX + ", " + mY;
	if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
		windowTitle += ", " + depth;
	}
	windowTitle += ", " + fpsString;
	window.setTitle(windowTitle);

	RowVector3f targetVec = camera.getPos() + camera.getForward();

	Matrix4f cameraMatrix = getPointAtMatrix(camera.getPos(), targetVec, camera.getUp());
	Matrix4f viewMatrix = cameraMatrix.inverse();

	int numberOnEachSideOfZero = 1;
	int distanceBetweenCubes = 3;
	for (int i = -numberOnEachSideOfZero; i <= numberOnEachSideOfZero; i++) {
		for (int j = -numberOnEachSideOfZero; j <= numberOnEachSideOfZero; j++) {
			render(objects[0], viewMatrix, (float)(i* distanceBetweenCubes), 0.0f, (float)(j * distanceBetweenCubes));
		}
	}

	//render(objects[0], viewMatrix, 0.0f, 0.0f, 4.0f);
	//render(objects[1], viewMatrix, 0.0f, 0.0f, 4.0f);
	//render(objects[2], viewMatrix, 35.0f, 0.0f, 4.0f);
	//render(objects[3], viewMatrix, 0.0f, 0.0f, 1.0f);
	//render(objects[4], viewMatrix, 0.0f, 0.0f, 1.0f);
}