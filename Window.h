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
	float* _depthBuffer;
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
	void setPixelDepth(int pixel, float depth);
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	int width();
	int height();
	int mX();
	int mY();
	string getTitle();
	float* getDepthBuffer();
	float getPixelDepth(int pixel);
	SDL_Window* get();
	SDL_Renderer* getRenderer();
	void drawPoint(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
	void init();
	void clear();
	void cleanup();
};

