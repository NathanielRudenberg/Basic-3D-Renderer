// Utility functions for transformations and stuff
#pragma once
#define PI 3.14159f
#include "Plane.h"
#include "triangle.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

// Get matrix used to move the camera around the world
mat4 getPointAtMatrix(const vec3& pos, const vec3& target, const vec3& up);

// Get translation matrix
mat4 getTranslationMatrix(vec3 pos);

// Get X rotation matrix
mat4 getXRot(float theta);

// Get Y rotation matrix
mat4 getYRot(float theta);

// Get Z rotation matrix
mat4 getZRot(float theta);

// Get a rotation matrix with combined angle rotations.
// Rotation order: X -> Y -> Z
mat4 getRotationMatrix(float thetaX, float thetaY, float thetaZ);

// Get projection matrix
mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

// Project a 3D vector onto a 2D plane
vec4 project(const vec4& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Get the intersection of a vector and a plane
vec4 vectorPlaneIntersect(Plane& plane, const vec4& lineStart, const vec4& lineEnd);

// Clip triangle against plane
int clipTriangleAgainstPlane(Plane& plane, Triangle& inTri, Triangle& outTri1, Triangle& outTri2);

// Transform a triangle through 3D space using a transformation matrix
void transformTriangle(Triangle& tri, const mat4& transformationMatrix);

// Project a triangle onto a 2D plane
void projectTriangle(Triangle& tri, int width, int height, float fov, Plane& nearPlane, Plane& farPlane);

// Project a triangle with a given view-projection matrix
void projectTriangle(Triangle& tri, const mat4& viewProjectionMatrix, int screenWidth, int screenHeight);

enum coordIndices {
	X,
	Y,
	Z,
	W
};