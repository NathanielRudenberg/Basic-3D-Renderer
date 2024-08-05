#include "Engine.h"

void Engine::OnLoop() {
	_window.clear();
	_renderer.render(objects);

	float fps = 1.0f / elapsedTime;
	string fpsString = "FPS: " + to_string(fps);
	string mX = "X: " + to_string(_window.mX());
	string mY = "Y: " + to_string(_window.mY());
	string depth = "Depth: " + to_string(_window.getDepthBuffer()[(_window.mY() * _window.width()) + _window.mX()]);
	string windowTitle = "Basic 3D Engine";
	if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
		windowTitle += " " + mX + ", " + mY + ", " + depth + ", ";
	}
	windowTitle += " " + fpsString;
	_window.setTitle(windowTitle);
}