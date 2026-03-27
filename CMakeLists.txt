cmake_minimum_required(VERSION 3.10)
project(3DEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Find GLM (header-only library)
find_package(glm REQUIRED)

# Source files
set(SOURCES
    Engine.cpp
    EngineEvent.cpp
    Engine_CheckKeystate.cpp
    Engine_OnCleanup.cpp
    Engine_OnEvent.cpp
    Engine_OnInit.cpp
    Engine_OnLoop.cpp
    Engine_OnRender.cpp
    Frustum.cpp
    mesh.cpp
    Model.cpp
    Plane.cpp
    Renderer.cpp
    Slope.cpp
    TransformUtilities.cpp
    Triangle.cpp
    Window.cpp
    camera.cpp
)

# Header files (for IDE support)
set(HEADERS
    Engine.h
    EngineEvent.h
    Frustum.h
    mesh.h
    Model.h
    Plane.h
    Renderer.h
    Slope.h
    TransformUtilities.h
    Triangle.h
    Window.h
    camera.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    glm::glm
)

# On Linux, we need to link pthread
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

# Enable additional warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
