#include "camera.h"
#include <Eigen/Geometry>

using Eigen::Quaternionf;

Camera::Camera(RowVector3f& pos, RowVector3f& forward, RowVector3f& up) : _pos(pos), _forward(forward.normalized()), _up(up.normalized()) {

}

RowVector3f Camera:: getPos() {
	return _pos;
}

RowVector3f Camera::getForward() {
	return _forward;
}

RowVector3f Camera::getLeft() {
	return _forward.cross(_up).normalized();
}

RowVector3f Camera::getRight() {
	return _up.cross(_forward).normalized();
}

RowVector3f Camera::getUp() {
	return _up;
}

void Camera::translate(RowVector3f& translateBy) {
	_pos = _pos + translateBy;
}

void Camera::rotate(float angle, RowVector3f& axis) {
	Quaternionf rotation{ {angle, axis} };

	_forward = rotation * _forward;
}

RowVector3f Camera::getHoriz() {
	return _forward.cross(_yAxis).normalized();
}

void Camera::rotateX(float angle) {
	RowVector3f horiz = getHoriz();
	rotate(angle, horiz);
	_up = horiz.cross(_forward).normalized();
}

void Camera::rotateY(float angle) {
	RowVector3f horiz = getHoriz();
	rotate(angle, _yAxis);
	_up = horiz.cross(_forward);
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