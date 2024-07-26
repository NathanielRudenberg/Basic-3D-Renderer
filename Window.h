#pragma once
#include <SDL.h>
#include <string>

using std::string;

class Window {
private:
	SDL_Window* _sdlWindow = NULL;
	SDL_Renderer* _sdlRenderer = NULL;
	int _screenWidth;
	int _screenHeight;
	double* _depthBuffer;
	int _depthBufferSize;
	string _title;
	int _xMousePos, _yMousePos;

public:
	Window();
	Window(string windowTitle, int screenWidth, int screenHeight);
	void setWidth(int width);
	void setHeight(int height);
	void setTitle(string windowTitle);
	void setMX(int pos);
	void setMY(int pos);
	int width();
	int height();
	int mX();
	int mY();
	string getTitle();
	double* getDepthBuffer();
	SDL_Window* get();
	SDL_Renderer* getRenderer();
	void init();
	void clear();
	void cleanup();
};

