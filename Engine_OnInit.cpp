#include "Engine.h"
#include <vector>

bool Engine::OnInit() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    
    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    /*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);*/
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create a window to use for display
    window = SDL_CreateWindow("Basic 3D Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) { std::cerr << "Failed to create window!" << std::endl; return false; }

    // Create an OpenGL context for rendering
    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) { std::cerr << "Could not create context!" << std::endl; return false; }

    // Initialize GLEW
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) { std::cerr << "Could not initialize GLEW!\n"; }

    // Initialize OpenGL
    if (!OnInitGL()) { std::cerr << "Could not initialize OpenGL!\n"; return NULL; }

    /*renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { return false; }*/

    Trigon s1, s2;
    s1.v[0] = { -0.5f, -0.5f, -0.5f, 1.0f }; s1.v[1] = { -0.5f, 0.5f, -0.5f, 1.0f }; s1.v[2] = { 0.5f, 0.5f, -0.5f, 1.0f };
    s2.v[0] = { -0.5f, -0.5f, -0.5f, 1.0f }; s2.v[1] = { 0.5f, 0.5f, -0.5f, 1.0f }; s2.v[2] = { 0.5f, -0.5f, -0.5f, 1.0f };

    Trigon e1, e2;
    e1.v[0] = { 0.5f, -0.5f, -0.5f, 1.0f }; e1.v[1] = { 0.5f, 0.5f, -0.5f, 1.0f }; e1.v[2] = { 0.5f, 0.5f, 0.5f, 1.0f };
    e2.v[0] = { 0.5f, -0.5f, -0.5f, 1.0f }; e2.v[1] = { 0.5f, 0.5f, 0.5f, 1.0f }; e2.v[2] = { 0.5f, -0.5f, 0.5f, 1.0f };

    Trigon n1, n2;
    n1.v[0] = { 0.5f, -0.5f, 0.5f, 1.0f }; n1.v[1] = { 0.5f, 0.5f, 0.5f, 1.0f }; n1.v[2] = { -0.5f, 0.5f, 0.5f, 1.0f };
    n2.v[0] = { 0.5f, -0.5f, 0.5f, 1.0f }; n2.v[1] = { -0.5f, 0.5f, 0.5f, 1.0f }; n2.v[2] = { -0.5f, -0.5f, 0.5f, 1.0f };

    Trigon w1, w2;
    w1.v[0] = { -0.5f, -0.5f, 0.5f, 1.0f }; w1.v[1] = { -0.5f, 0.5f, 0.5f, 1.0f }; w1.v[2] = { -0.5f, 0.5f, -0.5f, 1.0f };
    w2.v[0] = { -0.5f, -0.5f, 0.5f, 1.0f }; w2.v[1] = { -0.5f, 0.5f, -0.5f, 1.0f }; w2.v[2] = { -0.5f, -0.5f, -0.5f, 1.0f };

    Trigon t1, t2;
    t1.v[0] = { -0.5f, 0.5f, -0.5f, 1.0f }; t1.v[1] = { -0.5f, 0.5f, 0.5f, 1.0f }; t1.v[2] = { 0.5f, 0.5f, 0.5f, 1.0f };
    t2.v[0] = { -0.5f, 0.5f, -0.5f, 1.0f }; t2.v[1] = { 0.5f, 0.5f, 0.5f, 1.0f }; t2.v[2] = { 0.5f, 0.5f, -0.5f, 1.0f };

    Trigon b1, b2;
    b1.v[0] = { 0.5f, -0.5f, 0.5f, 1.0f }; b1.v[1] = { -0.5f, -0.5f, 0.5f, 1.0f }; b1.v[2] = { -0.5f, -0.5f, -0.5f, 1.0f };
    b2.v[0] = { 0.5f, -0.5f, 0.5f, 1.0f }; b2.v[1] = { -0.5f, -0.5f, -0.5f, 1.0f }; b2.v[2] = { 0.5f, -0.5f, -0.5f, 1.0f };

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

    virtCam = RowVector3f::Zero();

    return true;
}