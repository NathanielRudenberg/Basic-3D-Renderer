// Utility functions for transformations and stuff
#pragma once
#define PI 3.14159f
#include <Eigen/Core>
#include "triangle.h"
#include "Plane.h"

using Eigen::RowVector3f;
using Eigen::RowVector4f;
using Eigen::Matrix4f;

// Get matrix used to move the camera around the world
Matrix4f getPointAtMatrix(const RowVector3f& pos, const RowVector3f& target, const RowVector3f& up);

// Get translation matrix
Matrix4f getTranslationMatrix(float x, float y, float z);

// Get X rotation matrix
Matrix4f getXRot(float theta);

// Get Y rotation matrix
Matrix4f getYRot(float theta);

// Get Z rotation matrix
Matrix4f getZRot(float theta);

// Get a rotation matrix with combined angle rotations.
// Rotation order: X -> Y -> Z
Matrix4f getRotationMatrix(float thetaX, float thetaY, float thetaZ);

// Get projection matrix
Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Project a 3D vector onto a 2D plane
RowVector4f project(const RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Get the intersection of a vector and a plane
RowVector4f vectorPlaneIntersect(const RowVector3f& planePoint, const RowVector3f& planeNormal, const RowVector4f& lineStart, const RowVector4f& lineEnd);

// Clip triangle against plane
int clipTriangleAgainstPlane(const RowVector3f& planePoint, const RowVector3f& pN, Triangle& inTri, Triangle& outTri1, Triangle& outTri2);

// Transform a triangle through 3D space using a transformation matrix
void transformTriangle(Triangle& tri, const Matrix4f& transformationMatrix);

// Project a triangle onto a 2D plane
void projectTriangle(Triangle& tri, int width, int height, Plane& nearPlane, Plane& farPlane);

enum coordIndices {
	X,
	Y,
	Z,
	W
};