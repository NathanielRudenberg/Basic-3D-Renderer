#define PI 3.14159f

#include <Eigen/Geometry>
#include "Engine.h"
#include "TransformUtilities.h"

void Engine::OnLoop() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int numberOfCubesPerSide = 4;
	int calculatedNumOfCubesPerSide = numberOfCubesPerSide - 2;

	// Clear Z-buffer
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		depthBuffer[i] = INFINITY;
	}
	RowVector3f targetVec = camera.getPos() + camera.getForward();

	Matrix4f cameraMatrix = getPointAtMatrix(camera.getPos(), targetVec, camera.getUp());
	Matrix4f viewMatrix = cameraMatrix.inverse();

	//render(objects[0], viewMatrix, 0.0f, -0.5f, 0.0f);

	for (int i = -calculatedNumOfCubesPerSide; i <= calculatedNumOfCubesPerSide; i++) {
		for (int j = -calculatedNumOfCubesPerSide; j <= calculatedNumOfCubesPerSide; j++) {
			render(objects[1], viewMatrix, (float)(i*2), -1.0f, (float)(j*2));
		}
	}
}