#include "Engine.h"

void Engine::CheckKeystate() {
	cameraMoveSpeed = 2.0f;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	RowVector3f vForward = lookDir * (cameraMoveSpeed * elapsedTime);
	RowVector3f vRight = rightDir * (cameraMoveSpeed * elapsedTime);
	if (keystate[SDL_SCANCODE_SPACE]) {
		virtCam[Y] = virtCam[Y] + cameraMoveSpeed * elapsedTime;
	}
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		virtCam[Y] = virtCam[Y] - cameraMoveSpeed * elapsedTime;
	}
	if (keystate[SDL_SCANCODE_A]) {
		virtCam = virtCam + vRight;
	}
	if (keystate[SDL_SCANCODE_D]) {
		virtCam = virtCam - vRight;
	}
	if (keystate[SDL_SCANCODE_W]) {
		virtCam = virtCam + vForward;
	}
	if (keystate[SDL_SCANCODE_S]) {
		virtCam = virtCam - vForward;
	}
}