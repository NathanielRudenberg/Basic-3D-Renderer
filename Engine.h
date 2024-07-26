#pragma once
#include <vector>
#include <Eigen/Eigen>
#include "Renderer.h"
#include "EngineEvent.h"
#include "Window.h"
using std::string;
using std::to_string;

class Engine : public EngineEvent {
private:
	bool running;
	Window _window;
	Renderer _renderer;
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 700;
	std::vector<Model> objects;
	float elapsedTime = 0;

public:
	Engine();
	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void CheckKeystate();
	void OnExit();
	void OnKeyDown(SDL_Keycode sym, Uint16 mod);
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	void OnMButtonDown(int mX, int mY);
	void OnMButtonUp(int mX, int mY);
	void OnLoop();
	void OnRender();
	void OnCleanup();
};