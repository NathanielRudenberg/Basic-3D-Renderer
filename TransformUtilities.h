// Utility functions for transformations and stuff
#pragma once
#include "Engine.h"

// Get matrix used to move the camera around the world
Matrix4f getPointAtMatrix(RowVector3f& pos, RowVector3f& target, RowVector3f& up);

// Get translation matrix
Matrix4f getTranslationMatrix(float x, float y, float z);

// Get X rotation matrix
Matrix4f getXRot(float theta);

// Get Y rotation matrix
Matrix4f getYRot(float theta);

// Get Z rotation matrix
Matrix4f getZRot(float theta);

// Get projection matrix
Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Project a 3D vector onto a 2D plane
RowVector4f project(Eigen::RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);

// Get the intersection of a vector and a plane
RowVector4f vectorPlaneIntersect(RowVector3f& planeP, RowVector3f& planeN, RowVector4f& lineStart, RowVector4f& lineEnd);

// Clip triangle against plane
int clipTriangleAgainstPlane(RowVector3f& planePoint, RowVector3f& planeNormal, Engine::Trigon& inTri, Engine::Trigon& outTri1, Engine::Trigon& outTri2);