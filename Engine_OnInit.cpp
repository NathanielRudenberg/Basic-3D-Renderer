#include "Engine.h"
#include <vector>

bool Engine::OnInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    //depthBuffer = new float[SCREEN_WIDTH * SCREEN_HEIGHT];

    window = SDL_CreateWindow("Basic 3D Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) { return false; }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { return false; }

    //matExternal.loadObj("pyramid.obj");
    /*Model pyramid;
    pyramid.load("pyramid.obj");
    objects.push_back(pyramid);*/

    Model cube;
    cube.load("cube.obj");
    objects.push_back(cube);

    /*Model axis;
    axis.load("axis.obj");
    objects.push_back(axis);*/

    return true;
}