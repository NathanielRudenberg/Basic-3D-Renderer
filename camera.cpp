#include "camera.h"
#include <Eigen/Geometry>

using Eigen::Quaternionf;

Camera::Camera() : _pos(RowVector3f{ 0.0f, 0.0f, 0.0f }), _forward(RowVector3f{ 0.0f, 0.0f, 1.0f }), _up(RowVector3f{ 0.0f, 1.0f, 0.0f }) {}

Camera::Camera(RowVector3f& pos, RowVector3f& forward, RowVector3f& up) : _pos(pos), _forward(forward.normalized()), _up(up.normalized()) {}

RowVector3f& Camera:: getPos() {
	return _pos;
}

RowVector3f& Camera::getForward() {
	return _forward;
}

RowVector3f& Camera::getLeft() {
	_left = _up.cross(_forward).normalized();
	return _left;
}

RowVector3f& Camera::getRight() {
	_right = _forward.cross(_up).normalized();
	return _right;
}

RowVector3f& Camera::getUp() {
	return _up;
}

void Camera::translate(RowVector3f& translateBy, float amount, int dir) {
	if (dir == PLUS) {
		_pos = _pos + (translateBy * amount);
	}
	else if (dir == MINUS) {
		_pos = _pos - (translateBy * amount);
	}
}

void Camera::rotate(float angle, RowVector3f& axis) {
	Quaternionf rotation{ {angle, axis} };
	Eigen::Matrix3f rotMat = rotation.toRotationMatrix();

	_forward = _forward * rotMat;
}

RowVector3f Camera::getHoriz() {
	return _yAxis.cross(_forward).normalized();
}

void Camera::rotateX(float angle) {
	RowVector3f horiz = getHoriz();
	rotate(angle, horiz);
	_up = _forward.cross(horiz).normalized();
}

void Camera::rotateY(float angle) {
	RowVector3f horiz = getHoriz();
	rotate(angle, _yAxis);
	_up = _forward.cross(horiz).normalized();
}

void Camera::setPos(RowVector3f& pos) {
	_pos = pos;
}

void Camera::setForward(RowVector3f& forward) {
	_forward = forward;
}

void Camera::setUp(RowVector3f& up) {
	_up = up;
}