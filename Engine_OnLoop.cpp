#define PI 3.14159f

#include <Eigen/Geometry>
#include "Engine.h"
#include "TransformUtilities.h"

void Engine::OnLoop(float elapsedTime) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	lookDir = { 0.0f, 0.0f, 1.0f };
	RowVector4f tmpLook, tmpCamLoc;
	tmpCamLoc << virtCam, 1.0f;
	RowVector3f upVec{ 0.0f, 1.0f, 0.0f };
	RowVector4f tmpTarg = { 0.0f, 0.0f, 1.0f, 1.0f };
	Eigen::Quaternionf pitchChange;
	pitchChange = Eigen::AngleAxisf(pitch, rightDir.normalized());
	Matrix3f camRotX = pitchChange.toRotationMatrix();
	Matrix4f cameraRotationX;
	cameraRotationX << camRotX(0, 0), camRotX(0, 1), camRotX(0, 2), 0.0f,
		camRotX(1, 0), camRotX(1, 1), camRotX(1, 2), 0.0f,
		camRotX(2, 0), camRotX(2, 1), camRotX(2, 2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f;
	Matrix4f cameraRotationY = getYRot(yaw);
	tmpLook = tmpTarg * cameraRotationY * cameraRotationX;
	tmpTarg = tmpCamLoc + tmpLook;

	RowVector3f targetVec = virtCam + lookDir;

	lookDir[X] = tmpLook[X]; lookDir[Y] = tmpLook[Y]; lookDir[Z] = tmpLook[Z];
	targetVec[X] = tmpTarg[X]; targetVec[Y] = tmpTarg[Y]; targetVec[Z] = tmpTarg[Z];

	Matrix4f cameraMatrix = getPointAtMatrix(virtCam, targetVec, upVec, rightDir);
	Matrix4f viewMatrix = cameraMatrix.inverse();

	for (int i = -4; i < 4; i++) {
		for (int j = -4; j < 4; j++) {
			render(objects[0], viewMatrix, (float)i*2, -2.0f, (float)j*2);
		}
	}

	//render(objects[0], viewMatrix, 0.0f, 0.0f, 4.0f);
}