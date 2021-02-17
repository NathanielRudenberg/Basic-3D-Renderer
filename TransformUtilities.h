// Utility functions for transformations and stuff
#pragma once
#include "Engine.h"

// Get matrix used to move the camera around the world
Matrix4f getPointAtMatrix(RowVector4f& pos, RowVector4f& target, RowVector4f& up);

// Get X rotation matrix
Matrix4f getXRot(float theta);

// Get Y rotation matrix
Matrix4f getYRot(float theta);

// Get Z rotation matrix
Matrix4f getZRot(float theta);

// Get projection matrix
Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane);

RowVector4f project(Eigen::RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);