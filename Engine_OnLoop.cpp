#include "Engine.h"

void Engine::OnLoop() {
	_window.clear();

	RowVector3f targetVec = _renderer.camera().getPos() + _renderer.camera().getForward();
	Matrix4f cameraMatrix = getPointAtMatrix(_renderer.camera().getPos(), targetVec, _renderer.camera().getUp());
	Matrix4f viewMatrix = cameraMatrix.inverse();

	for (Model& object : objects) {
		_renderer.render(object, viewMatrix);
	}

	//_renderer.render(objects[0], viewMatrix);
	//render(objects[1], viewMatrix, 0.0f, 0.0f, 4.0f);
	//render(objects[2], viewMatrix, 35.0f, 0.0f, 4.0f);
	//render(objects[3], viewMatrix, 0.0f, 0.0f, 1.0f);
	//render(objects[4], viewMatrix, 0.0f, 0.0f, 1.0f);

	float fps = 1.0f / elapsedTime;
	string fpsString = "FPS: " + to_string(fps);
	string mX = "X: " + to_string(_window.mX());
	string mY = "Y: " + to_string(_window.mY());
	string depth = "Depth: " + to_string( _window.getDepthBuffer()[(_window.mY() * SCREEN_WIDTH) + _window.mX()]);
	string windowTitle = "Basic 3D Engine";
	if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
		windowTitle += " " + mX + ", " + mY + ", " + depth + ", ";
	}
	windowTitle += " " + fpsString;
	_window.setTitle(windowTitle);
}