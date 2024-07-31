// Utility functions for transformations and stuff
#pragma once
#define PI 3.14159f
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "triangle.h"
#include "Plane.h"

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
mat4 getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Project a 3D vector onto a 2D plane
vec4 project(const vec4& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Get the intersection of a vector and a plane
vec4 vectorPlaneIntersect(const vec3& planePoint, const vec3& planeNormal, const vec4& lineStart, const vec4& lineEnd);

// Clip triangle against plane
int clipTriangleAgainstPlane(const vec3& planePoint, const vec3& pN, Triangle& inTri, Triangle& outTri1, Triangle& outTri2);

// Transform a triangle through 3D space using a transformation matrix
void transformTriangle(Triangle& tri, const mat4& transformationMatrix);

// Project a triangle onto a 2D plane
void projectTriangle(Triangle& tri, int width, int height, Plane& nearPlane, Plane& farPlane);

enum coordIndices {
	X,
	Y,
	Z,
	W
};