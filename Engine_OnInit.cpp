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
    /*Model pyramid = Model("pyramid.obj");
    objects.push_back(pyramid);*/

    /*Model cube = Model("cube.obj", 0.0f, 0.0f, 4.0f);
    objects.push_back(cube);*/
    //objectBuffer.push_back(&cube);

    int numberOnEachSideOfZero = 1;
    float distanceBetweenCubes = 2.5;
    for (int i = -numberOnEachSideOfZero; i <= numberOnEachSideOfZero; i++) {
        for (int j = -numberOnEachSideOfZero; j <= numberOnEachSideOfZero; j++) {
            for (int k = -numberOnEachSideOfZero; k <= numberOnEachSideOfZero; k++) {
                objects.push_back(Model("cube.obj", (float)(i * distanceBetweenCubes), (float)(k * distanceBetweenCubes), (float)(j * distanceBetweenCubes)));
            }
        }
    }

    /*Model axis = Model("axis.obj");
    objects.push_back(axis);*/

    /*Model ship = Model("VideoShip.obj");
    objects.push_back(ship);

    Model teapot = Model("teapot.obj");
    objects.push_back(teapot);*/

    /*Model mountains = Model("mountains.obj", 0.0f, -12.0f, 0.0f);
    objects.push_back(mountains);*/

    return true;
}