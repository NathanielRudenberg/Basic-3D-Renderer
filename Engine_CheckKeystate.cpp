#include "Engine.h"

void Engine::CheckKeystate() {
	float cameraMoveSpeed = 2.0f;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_LCTRL]) {
		cameraMoveSpeed = 8.0f;
	}
	if (keystate[SDL_SCANCODE_SPACE]) {
		camera.translate(camera._yAxis, (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		camera.translate(camera._yAxis, (cameraMoveSpeed * elapsedTime), Camera::MINUS);
	}
	if (keystate[SDL_SCANCODE_A]) {
		camera.translate(camera.getLeft(), (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_D]) {
		camera.translate(camera.getRight(), (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_W]) {
		camera.translate(camera.getFront(), (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_S]) {
		camera.translate(camera.getBack(), (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_UP]) {
		camera.translate(camera.getForward(), (cameraMoveSpeed * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_DOWN]) {
		camera.translate(camera.getForward(), (cameraMoveSpeed * elapsedTime), Camera::MINUS);
	}
}