#include "camera.h"
#include <cmath>

Camera::Camera() : _pos({ 0.0f, 0.0f, 0.0f }), _forward({ 0.0f, 0.0f, 1.0f }), _up({ 0.0f, 1.0f, 0.0f }), _fov(80.0f) {}

Camera::Camera(const vec3& pos, const vec3& forward, const vec3& up) : _pos(pos), _forward(normalize(forward)), _up(normalize(up)) {}

vec3& Camera:: getPos() {
	return _pos;
}

vec3& Camera::getForward() {
	return _forward;
}

vec3& Camera::getLeft() {
	_left = normalize(cross(_up, _forward));
	return _left;
}

vec3& Camera::getRight() {
	_right = normalize(cross(_forward, _up));
	return _right;
}

vec3& Camera::getUp() {
	return _up;
}

vec3& Camera::getFront() {
	_front = cross(_yAxis, getRight());
	return _front;
}

vec3& Camera::getBack() {
	_back = cross(getRight(), _yAxis);
	return _back;
}

float Camera::fov() {
	return _fov;
}

float Camera::fovRad() {
	return (_fov / 180.0f) * PI;
}

float Camera::inverseFovRad() {
	return 1.0f / tanf(fovRad() * 0.5);
}

void Camera::translate(const vec3& translateBy, float amount, int dir) {
	if (dir == PLUS) {
		_pos = _pos + (translateBy * amount);
	}
	else if (dir == MINUS) {
		_pos = _pos - (translateBy * amount);
	}
}

void Camera::rotate(float angle, const vec3& axis) {
	quat rotation = quat(angleAxis(angle, axis));
	_forward = normalize(_forward * rotation);
	_up = normalize(_up * rotation);
}

vec3 Camera::getHoriz() {
	return normalize(cross(_up, _forward));
}

void Camera::rotateX(float angle) {
	vec3 horiz = getHoriz();
	rotate(angle, horiz);
}

void Camera::rotateY(float angle) {
	// Change the rotation direction based on the sign of the
	// up vector's Y component
	int sign = (int)(_up[Y] / std::abs(_up[Y]));
	rotate(angle * sign, _yAxis);
}

void Camera::setPos(const vec3& pos) {
	_pos = pos;
}

void Camera::setForward(const vec3& forward) {
	_forward = forward;
}

void Camera::setUp(const vec3& up) {
	_up = up;
}

void Camera::setFov(float fov) {
	_fov = fov;
}