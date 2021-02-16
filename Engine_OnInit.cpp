#define PI 3.14159f

#include "Engine.h"
#include <vector>

bool Engine::OnInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Basic 3D Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) { return false; }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { return false; }

    Trigon s1, s2;
    s1.v[0] = { -0.5f, -0.5f, -0.5f }; s1.v[1] = { -0.5f, 0.5f, -0.5f }; s1.v[2] = { 0.5f, 0.5f, -0.5f };
    s2.v[0] = { -0.5f, -0.5f, -0.5f }; s2.v[1] = { 0.5f, 0.5f, -0.5f }; s2.v[2] = { 0.5f, -0.5f, -0.5f };

    Trigon e1, e2;
    e1.v[0] = { 0.5f, -0.5f, -0.5f }; e1.v[1] = { 0.5f, 0.5f, -0.5f }; e1.v[2] = { 0.5f, 0.5f, 0.5f };
    e2.v[0] = { 0.5f, -0.5f, -0.5f }; e2.v[1] = { 0.5f, 0.5f, 0.5f }; e2.v[2] = { 0.5f, -0.5f, 0.5f };

    Trigon n1, n2;
    n1.v[0] = { 0.5f, -0.5f, 0.5f }; n1.v[1] = { 0.5f, 0.5f, 0.5f }; n1.v[2] = { -0.5f, 0.5f, 0.5f };
    n2.v[0] = { 0.5f, -0.5f, 0.5f }; n2.v[1] = { -0.5f, 0.5f, 0.5f }; n2.v[2] = { -0.5f, -0.5f, 0.5f };

    Trigon w1, w2;
    w1.v[0] = { -0.5f, -0.5f, 0.5f }; w1.v[1] = { -0.5f, 0.5f, 0.5f }; w1.v[2] = { -0.5f, 0.5f, -0.5f };
    w2.v[0] = { -0.5f, -0.5f, 0.5f }; w2.v[1] = { -0.5f, 0.5f, -0.5f }; w2.v[2] = { -0.5f, -0.5f, -0.5f };

    Trigon t1, t2;
    t1.v[0] = { -0.5f, 0.5f, -0.5f }; t1.v[1] = { -0.5f, 0.5f, 0.5f }; t1.v[2] = { 0.5f, 0.5f, 0.5f };
    t2.v[0] = { -0.5f, 0.5f, -0.5f }; t2.v[1] = { 0.5f, 0.5f, 0.5f }; t2.v[2] = { 0.5f, 0.5f, -0.5f };

    Trigon b1, b2;
    b1.v[0] = { 0.5f, -0.5f, 0.5f }; b1.v[1] = { -0.5f, -0.5f, 0.5f }; b1.v[2] = { -0.5f, -0.5f, -0.5f };
    b2.v[0] = { 0.5f, -0.5f, 0.5f }; b2.v[1] = { -0.5f, -0.5f, -0.5f }; b2.v[2] = { 0.5f, -0.5f, -0.5f };

    matCube.tris.push_back(s1);
    matCube.tris.push_back(s2);
    matCube.tris.push_back(e1);
    matCube.tris.push_back(e2);
    matCube.tris.push_back(n1);
    matCube.tris.push_back(n2);
    matCube.tris.push_back(w1);
    matCube.tris.push_back(w2);
    matCube.tris.push_back(t1);
    matCube.tris.push_back(t2);
    matCube.tris.push_back(b1);
    matCube.tris.push_back(b2);

    matExternal.loadObj("teapot.obj");

    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float fov = 90.0f;
    float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
    float aspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;

    projMat(0, 0) = aspectRatio * fovRad;
    projMat(1, 1) = fovRad * -1.0f;
    projMat(2, 2) = farPlane / (farPlane - nearPlane);
    projMat(3, 2) = (-farPlane * nearPlane) / (farPlane - nearPlane);
    projMat(2, 3) = 1.0f;

    return true;
}