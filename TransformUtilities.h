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

RowVector4f project(Eigen::RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane);