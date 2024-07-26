#include "Engine.h"

void Engine::CheckKeystate() {
	_renderer.setCameraMoveSpeed(2.0f);
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_LCTRL]) {
		_renderer.setCameraMoveSpeed(8.0f);
	}
	if (keystate[SDL_SCANCODE_E]) {
		_renderer.setCameraMoveSpeed(0.1f);
	}
	if (keystate[SDL_SCANCODE_SPACE]) {
		_renderer.camera().translate(_renderer.camera()._yAxis, (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		_renderer.camera().translate(_renderer.camera()._yAxis, (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::MINUS);
	}
	if (keystate[SDL_SCANCODE_A]) {
		_renderer.camera().translate(_renderer.camera().getLeft(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_D]) {
		_renderer.camera().translate(_renderer.camera().getRight(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_W]) {
		_renderer.camera().translate(_renderer.camera().getFront(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_S]) {
		_renderer.camera().translate(_renderer.camera().getBack(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_UP]) {
		_renderer.camera().translate(_renderer.camera().getForward(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::PLUS);
	}
	if (keystate[SDL_SCANCODE_DOWN]) {
		_renderer.camera().translate(_renderer.camera().getForward(), (_renderer.getCameraMoveSpeed() * elapsedTime), Camera::MINUS);
	}
}