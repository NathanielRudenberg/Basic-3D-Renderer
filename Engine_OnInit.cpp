#include "Engine.h"

bool Engine::OnInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    _window = Window("Basic 3D Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    _renderer = Renderer(&_window, 0.2f, 2.0f);
    _window.init();
    if (_window.get() == NULL) { return false; }
    if (_window.getRenderer() == NULL) { return false; }

    //matExternal.loadObj("pyramid.obj");
    /*Model pyramid;
    pyramid.load("pyramid.obj");
    objects.push_back(pyramid);*/

    /*Model cube = Model("cube.obj", 0.0f, 0.0f, 4.0f);
    objects.push_back(cube);
    objectBuffer.push_back(&cube);*/

    int numberOnEachSideOfZero = 1;
    int distanceBetweenCubes = 3;
    for (int i = -numberOnEachSideOfZero; i <= numberOnEachSideOfZero; i++) {
        for (int j = -numberOnEachSideOfZero; j <= numberOnEachSideOfZero; j++) {
            objects.push_back(Model("cube.obj", (float)(i * distanceBetweenCubes), 0.0f, (float)(j * distanceBetweenCubes)));
        }
    }

    Model axis = Model("axis.obj");
    objects.push_back(axis);

    /*Model ship;
    ship.load("VideoShip.obj");
    objects.push_back(ship);

    Model teapot;
    teapot.load("teapot.obj");
    objects.push_back(teapot);

    Model mountains;
    mountains.load("mountains.obj");
    objects.push_back(mountains);*/

    return true;
}