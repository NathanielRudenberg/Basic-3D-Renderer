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

void Window::setPixelDepth(int pixel, float depth) {
	_depthBuffer[pixel] = depth;
}

void Window::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(_sdlRenderer, r, g, b, a);
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

float* Window::getDepthBuffer() {
	return _depthBuffer;
}

float Window::getPixelDepth(int pixel){
	return _depthBuffer[pixel];
}

SDL_Window* Window::get() {
	return _sdlWindow;
}

SDL_Renderer* Window::getRenderer() {
	return _sdlRenderer;
}

void Window::drawPoint(int x, int y) {
	SDL_RenderDrawPoint(_sdlRenderer, x, y);
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(_sdlRenderer, x1, y1, x2, y2);
}

void Window::init() {
	_sdlWindow = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width(), height(), SDL_WINDOW_SHOWN);
	_sdlRenderer = SDL_CreateRenderer(get(), -1, SDL_RENDERER_ACCELERATED);
	_depthBuffer = new float[_depthBufferSize] {INFINITY};
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
