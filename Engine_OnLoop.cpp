#define PI 3.14159f

#include <Eigen/Geometry>
#include "Engine.h"
#include "TransformUtilities.h"

void Engine::OnLoop() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	RowVector3f targetVec = camera.getPos() + camera.getForward();

	Matrix4f cameraMatrix = getPointAtMatrix(camera.getPos(), targetVec, camera.getUp());
	Matrix4f viewMatrix = cameraMatrix.inverse();

	for (int i = -1; i <= 0; i++) {
		for (int j = -1; j <= 0; j++) {
			render(objects[0], viewMatrix, (float)(i*15), 0.0f, (float)(j*15));
		}
	}

	//render(objects[0], viewMatrix, 0.0f, 0.0f, 0.0f);
}