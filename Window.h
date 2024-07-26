#pragma once
#include <SDL.h>
#include <string>

using std::string;

class Window {
private:
	SDL_Window* _window = NULL;
	SDL_Renderer* _renderer = NULL;
	int _screenWidth;
	int _screenHeight;
	double* _depthBuffer;
	int _depthBufferSize;
	string _title;

public:
	Window();
	Window(string windowTitle, int screenWidth, int screenHeight);
	void setScreenWidth(int width);
	void setScreenHeight(int height);
	void setTitle(string windowTitle);
	int getWidth();
	int getHeight();
	string getTitle();
	double* getDepthBuffer();
	SDL_Window* get();
	SDL_Renderer* getRenderer();
	void init();
	void clear();
	void cleanup();
};

