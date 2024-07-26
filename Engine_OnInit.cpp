#include "Engine.h"

bool Engine::OnInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    _window = Window("Basic 3D Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    _renderer = Renderer(_window, 0.2f, 2.0f);
    _window.init();
    if (_window.get() == NULL) { return false; }
    if (_window.getRenderer() == NULL) { return false; }

    //matExternal.loadObj("pyramid.obj");
    /*Model pyramid;
    pyramid.load("pyramid.obj");
    objects.push_back(pyramid);*/

    Model cube;
    cube.load("cube.obj");
    objects.push_back(cube);

    Model axis;
    axis.load("axis.obj");
    objects.push_back(axis);

    Model ship;
    ship.load("VideoShip.obj");
    objects.push_back(ship);

    Model teapot;
    teapot.load("teapot.obj");
    objects.push_back(teapot);

    Model mountains;
    mountains.load("mountains.obj");
    objects.push_back(mountains);

    return true;
}