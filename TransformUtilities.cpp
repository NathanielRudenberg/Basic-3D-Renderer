#include "TransformUtilities.h"

Matrix4f getPointAtMatrix(RowVector3f& pos, RowVector3f& target, RowVector3f& up) {
	RowVector3f newForward = (target - pos).normalized();
	RowVector3f a = newForward * up.dot(newForward);
	RowVector3f newUp = (up - a).normalized();
	RowVector3f newRight = newUp.cross(newForward);

	Matrix4f pointAt;
	pointAt <<	newRight, 0,
				newUp, 0,
				newForward, 0,
				pos, 1;

	return pointAt;
}

Matrix4f getXRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = 1.0f;
	m(1, 1) = cosf(theta);
	m(1, 2) = sinf(theta);
	m(2, 1) = -sinf(theta);
	m(2, 2) = cosf(theta);
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getYRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = cosf(theta);
	m(0, 2) = -sinf(theta);
	m(1, 1) = 1.0f;
	m(2, 0) = sinf(theta);
	m(2, 2) = cosf(theta);
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getZRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = cosf(theta);
	m(0, 1) = sinf(theta);
	m(1, 0) = -sinf(theta);
	m(1, 1) = cosf(theta);
	m(2, 2) = 1.0f;
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	Matrix4f projMat = Matrix4f::Zero();
	projMat(0, 0) = aspectRatio * fovRadians;
	projMat(1, 1) = fovRadians * -1.0f;
	projMat(2, 2) = farPlane / (farPlane - nearPlane);
	projMat(3, 2) = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMat(2, 3) = 1.0f;

	return projMat;
}