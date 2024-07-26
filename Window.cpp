#include "Window.h"

Window::Window() : _title(""), _screenWidth(0), _screenHeight(0), _depthBuffer(nullptr), _depthBufferSize(0) {
}

Window::Window(string windowTitle, int screenWidth, int screenHeight) : _title(windowTitle), _screenWidth(screenWidth), _screenHeight(screenHeight) {
	_depthBufferSize = _screenWidth * _screenHeight;
	_depthBuffer = nullptr;
}

void Window::setScreenWidth(int width) {
	_screenWidth = width;
}

void Window::setScreenHeight(int height) {
	_screenHeight = height;
}

void Window::setTitle(string windowTitle) {
	SDL_SetWindowTitle(get(), windowTitle.c_str());
}

int Window::getWidth() {
	return _screenWidth;
}

int Window::getHeight() {
	return _screenHeight;
}

string Window::getTitle() {
	return _title;
}

double* Window::getDepthBuffer() {
	return _depthBuffer;
}

SDL_Window* Window::get() {
	return _window;
}

SDL_Renderer* Window::getRenderer() {
	return _renderer;
}

void Window::init() {
	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getWidth(), getHeight(), SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(get(), -1, SDL_RENDERER_ACCELERATED);
	_depthBuffer = new double[_depthBufferSize] {INFINITY};
}

void Window::clear() {
	SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 0);
	SDL_RenderClear(getRenderer());
	for (int i = 0; i < _depthBufferSize; i++) {
		getDepthBuffer()[i] = INFINITY;
	}
}

void Window::cleanup() {
	SDL_DestroyWindow(get());
	SDL_DestroyRenderer(getRenderer());
}




