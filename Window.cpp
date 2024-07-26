#include "Window.h"

Window::Window() :
	_title(""), _screenWidth(0), _screenHeight(0), _depthBuffer(nullptr), _depthBufferSize(0), _xMousePos(0), _yMousePos(0) {
}

Window::Window(string windowTitle, int screenWidth, int screenHeight) :
	_title(windowTitle), _screenWidth(screenWidth), _screenHeight(screenHeight), _xMousePos(0), _yMousePos(0) {
	_depthBufferSize = _screenWidth * _screenHeight;
	_depthBuffer = nullptr;
}

void Window::setWidth(int width) {
	_screenWidth = width;
}

void Window::setHeight(int height) {
	_screenHeight = height;
}

void Window::setTitle(string windowTitle) {
	SDL_SetWindowTitle(get(), windowTitle.c_str());
}

void Window::setMX(int pos) {
	_xMousePos = pos;
}

void Window::setMY(int pos) {
	_yMousePos = pos;
}

int Window::width() {
	return _screenWidth;
}

int Window::height() {
	return _screenHeight;
}

int Window::mX() {
	return _xMousePos;
}

int Window::mY() {
	return _yMousePos;
}

string Window::getTitle() {
	return _title;
}

double* Window::getDepthBuffer() {
	return _depthBuffer;
}

SDL_Window* Window::get() {
	return _sdlWindow;
}

SDL_Renderer* Window::getRenderer() {
	return _sdlRenderer;
}

void Window::init() {
	_sdlWindow = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width(), height(), SDL_WINDOW_SHOWN);
	_sdlRenderer = SDL_CreateRenderer(get(), -1, SDL_RENDERER_ACCELERATED);
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
